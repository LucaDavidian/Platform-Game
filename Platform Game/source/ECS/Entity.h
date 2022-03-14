#ifndef ENTITY_H
#define ENTITY_H

#include "../utility/Bitset.h"

#include "../utility/Utility.hpp"
#include "Component.h"
#include "../event system/EventBus.hpp"

class Entity
{
public:
	Entity() : mIsAlive(true), mIsActive(true) 
	{ 
		for (size_t i = 0; i < MAX_COMPONENTS; i++) 
			mComponents[i] = nullptr; 
	}
	
	~Entity() 
	{ 
		for (Component *component : mComponents) 
			delete component; 
	}

	template <typename T, typename U = T, typename... Args>
	T *AddComponent(Args&&... args);

	template <typename T, typename U = T>
	T *AddComponent(T *component);

	template <typename T>
	bool HasComponent() const;

	template <typename... Types>
	bool HasComponents();

	template <typename T>
	T *GetComponent() const;

	template <typename T>
	T *GetComponentSafe() const;

	bool IsAlive() const { return mIsAlive; }
	void Destroy() { mIsAlive = false; }

	bool IsActive() const { return mIsActive; }
	void SetActive(bool active) { mIsActive = active; }

	EventBus &GetEventBus() { return mEventBus; }

	void Init() const
	{
		for (auto component : mComponents)
			if (component)
				component->Init();
	}
private:
	static const unsigned int MAX_COMPONENTS = 32;
	Component *mComponents[MAX_COMPONENTS];  // Entity owns components 
	
	Bitset<MAX_COMPONENTS> mComponentMask;

	template <typename T, typename... Types>
	bool HasComponentsImpl();

	bool mIsAlive;
	bool mIsActive;

	EventBus mEventBus;
};

template <typename T, typename U /* = T */, typename... Args>
T *Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<U,T>::value, "T is not a component derived from U");

	T *component = new T(Utility::Forward<Args>(args)...);

	component->SetOwner(this);

	mComponents[GetComponentID<U>()] = component;  
	
	if (!mComponentMask.Test(GetComponentID<U>()))
		mComponentMask.Set(GetComponentID<U>());

	//component->Init();   // Init called on entity creation (#TODO OnComponentAdded, OnEntityCreated)

	return component;
}

template <typename T, typename U /* = T */>
T *Entity::AddComponent(T *component)
{
	static_assert(std::is_base_of<U,T>::value, "T is not a component derived from U");

	mComponents[GetComponentID<U>()] = component;  
	
	if (!mComponentMask.Test(GetComponentID<U>()))
		mComponentMask.Set(GetComponentID<U>());

	component->Init();

	return component;
}

template <typename T>
bool Entity::HasComponent() const
{
	//return mComponentMask[GetComponentID<T>()];
	return mComponents[GetComponentID<T>()] != nullptr;
}

template <>
inline bool Entity::HasComponents() // base case for recursive HasComponents implementation
{
	return true;
}

template <typename... Types>
bool Entity::HasComponents()
{
	//return (mComponentMask[GetComponentID<Types>()] && ... && true);     // binary (right) fold expression (C++17)
	//return (mComponents[GetComponentID<Types>()] && ... && true);        // binary (right) fold expression
	//return (... && mComponents[GetComponentID<Types>()]);                // unary (left) fold expression
	//return (mComponents[GetComponentID<Types>()] && ...);                // unary (right) fold expression

	return HasComponentsImpl<Types...>();
}

template <typename T, typename... Types>
bool Entity::HasComponentsImpl()
{
	return HasComponent<T>() && HasComponents<Types...>();
}

template <typename T>
T *Entity::GetComponent() const
{
	return static_cast<T*>(mComponents[GetComponentID<T>()]);
}

#endif  // ENTITY_H