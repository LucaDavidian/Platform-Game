#include "TransformAnimationComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	TransformAnimationComponent TransformAnimationComponentTableCtor(LuaObject componentTable)
	{
		Vector<TransformAnimationComponent::Animation::KeyFrame> keyFrames;

		if (auto animationTable = componentTable.Get("animation"); animationTable)
		{
			while (animationTable.HasNext())
			{
				auto keyFramePair = animationTable.GetNext();
				auto keyFrametable = keyFramePair.second;
				
				glm::vec3 keyFramePosition(keyFrametable.Get(1).Get(1), keyFrametable.Get(1).Get(2), keyFrametable.Get(1).Get(3));
				glm::vec3 keyFrameRotation(keyFrametable.Get(2).Get(1), keyFrametable.Get(2).Get(2), keyFrametable.Get(2).Get(3));
				glm::vec3 keyFrameScale(keyFrametable.Get(3).Get(1), keyFrametable.Get(3).Get(2), keyFrametable.Get(3).Get(3));

				float keyFrameTime = keyFrametable.Get(4);

				keyFrames.InsertLast(TransformAnimationComponent::Animation::KeyFrame{ keyFramePosition, keyFrameRotation, keyFrameScale, keyFrameTime });
			}
		}

		bool animationLoop = false;

		if (auto loop = componentTable.Get("loop"); loop)
			animationLoop = loop;

		TransformAnimationComponent transformAnimationComponent;
		transformAnimationComponent.PlayAnimation(keyFrames, animationLoop);

		return transformAnimationComponent;
	}

	void TransformAnimationComponentAddToEntity(TransformAnimationComponent const &transformAnimationComponent, Entity &entity)
	{
		entity.AddComponent<TransformAnimationComponent>(transformAnimationComponent);
	}
}

const bool TransformAnimationComponent::mRegistered = []() {
	Reflect::Reflect<TransformAnimationComponent>("transform_animation")
		.AddConstructor(TransformAnimationComponentTableCtor)
		.AddMemberFunction(TransformAnimationComponentAddToEntity, "add_to_entity");

	return true;
}();
