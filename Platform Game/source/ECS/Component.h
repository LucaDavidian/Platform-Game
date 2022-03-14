#ifndef COMPONENT_H
#define COMPONENT_H

#include <type_traits>

inline unsigned int GetUniqueID()
{
	static unsigned id = 0;

	return id++;
}

class Component;

template <typename T>
constexpr unsigned int GetComponentID()
{
	static_assert(std::is_base_of<Component, T>::value, "T is not a component type");

	static const unsigned int componentID = GetUniqueID();

	return componentID;
}

class Entity;

class Component
{
public:
	virtual ~Component() = default;

	virtual void Init() {}

	void SetOwner(Entity *entity) { mOwner = entity;  }
	Entity *GetOwner() const { return mOwner; }

protected:
	Component() : mOwner(nullptr) {}

private:
	Entity *mOwner;
};

#endif  // COMPONENT_H