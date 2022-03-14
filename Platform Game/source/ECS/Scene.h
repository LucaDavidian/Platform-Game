#ifndef SCENE_H
#define SCENE_H

#include "../data structures/vector.hpp"
#include "Entity.h"

class Scene
{
public:
	static Scene &GetInstance() { static Scene instance; return instance; }

	Entity *AddEntity();

	const Vector<Entity*> &GetEntities() const { return mEntities; }

	void Update();  
	
	Entity *GetCamera() const { return mCamera; }
	void SetCamera(Entity *entity) { mCamera = entity; }  // #TODO: camera system

	bool HasEntity(Entity *entityToCheck)   // for game states #TODO_hacky
	{
		for (Entity *entity : mEntities)
			if (entityToCheck == entity)
				return true;

		return false;
	}
private:
	Scene() : mCamera(nullptr), mPlayer(nullptr) {}

	void RemoveEntity(Entity *entity);

	Vector<Entity*> mEntities;  // Scene owns entities
	Entity *mCamera;
	Entity *mPlayer;
};

#endif  // SCENE_H