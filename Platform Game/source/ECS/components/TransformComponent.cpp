#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	TransformComponent TransformComponentTableCtor(LuaObject transformComponentTable)
	{
		LuaObject positionTable = transformComponentTable.Get("position");
		LuaObject rotation = transformComponentTable.Get("rotation");
		LuaObject scaleTable = transformComponentTable.Get("scale");

		TransformComponent transformComponent;
		transformComponent.SetPosition(positionTable.Get(1), positionTable.Get(2), positionTable.Get(3));
		transformComponent.SetOrientation(0.0f, 0.0f, glm::radians(rotation.Cast<float>()));
		transformComponent.SetScale(scaleTable.Get(1), scaleTable.Get(2), scaleTable.Get(3));

		return transformComponent;
	}

	void TransformComponentAddToEntity(TransformComponent transformComponent, Entity &entity)
	{
		entity.AddComponent<TransformComponent>(transformComponent);
	}
}

const bool TransformComponent::mRegistered = []() {
	Reflect::Reflect<TransformComponent>("transform")
		.AddConstructor(TransformComponentTableCtor)
		.AddMemberFunction(TransformComponentAddToEntity, "add_to_entity");

	return true;
}();


TransformComponent::TransformComponent() 
	: mPosition(glm::vec3()), mOrientation(glm::vec3()), mScale(glm::vec3(1.0f))
{
	UpdateWorldMatrix();
}

void TransformComponent::UpdateWorldMatrix()
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mScale.x, mScale.y, mScale.z));

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), mOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, mOrientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, mOrientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mPosition.x, mPosition.y, mPosition.z));

	mWorldMatrix = translationMatrix * rotationMatrix;
	mScaledWorldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	mDirty = false;
}

glm::mat4 TransformComponent::GetWorldMatrix() 
{ 
	if (mDirty) 
		UpdateWorldMatrix(); 
	
	return mWorldMatrix; 
}

glm::mat4 TransformComponent::GetScaledWorldMatrix() 
{ 
	if (mDirty) 
		UpdateWorldMatrix(); 
	
	return mScaledWorldMatrix; 
}

glm::mat4 TransformComponent::GetInverseWorldMatrix() 
{ 
	if (mDirty) 
	{ 
		UpdateWorldMatrix(); 
		mInverseWorldMatrix = glm::inverse(mWorldMatrix); 
	} 

	return mInverseWorldMatrix; 
}

glm::vec3 TransformComponent::GetAxisX()
{
	if (mDirty)
		UpdateWorldMatrix();

	return glm::vec3(mWorldMatrix[0][0], mWorldMatrix[0][1], mWorldMatrix[0][2]);    // glm stores matrices in colum-major order
}

glm::vec3 TransformComponent::GetAxisY()
{
	if (mDirty)
		UpdateWorldMatrix();

	return glm::vec3(mWorldMatrix[1][0], mWorldMatrix[1][1], mWorldMatrix[1][2]);
}

glm::vec3 TransformComponent::GetAxisZ()
{
	if (mDirty)
		UpdateWorldMatrix();

	return glm::vec3(mWorldMatrix[2][0], mWorldMatrix[2][1], mWorldMatrix[2][2]);
}