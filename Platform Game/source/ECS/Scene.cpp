#include "Scene.h"
#include <algorithm>

Entity *Scene::AddEntity() 
{ 
	Entity *entity = new Entity; 
	mEntities.InsertLast(entity); 
	
	return entity; 
}

void Scene::RemoveEntity(Entity *entity)
{
	auto it = mEntities.Find(entity);

	if (it != mEntities.End())
		mEntities.Remove(it);
}

void Scene::Update()
{
	if (!mEntities.Size())
		return;

	// remove dead entities
	Vector<Entity*>::Iterator it = std::remove_if(mEntities.Begin(), mEntities.End(), [](Entity *entity) { if (entity->IsAlive()) return false; else { delete entity; return true; } });
	mEntities.Remove(it, mEntities.End());
}
