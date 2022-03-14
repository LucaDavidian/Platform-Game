#include "SpriteAnimationComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	SpriteAnimationComponent SpriteAnimationComponentTableCtor(LuaObject componentTable)
	{
		SpriteAnimationComponent spriteAnimationComponent;

		LuaObject animationsTable = componentTable.Get("animations");

		while (animationsTable.HasNext())
		{
			std::pair<LuaObject, LuaObject> animationPair = animationsTable.GetNext();

			LuaObject animationTable = animationPair.second;

			Vector<SpriteAnimationComponent::Animation::KeyFrame> keyFrames;

			while (animationTable.HasNext())
			{
				auto keyFrameTable = animationTable.GetNext().second;

				keyFrames.EmplaceLast(keyFrameTable.Get(1), keyFrameTable.Get(2));
			}

			spriteAnimationComponent.AddAnimation(animationPair.first, { keyFrames });
		}

		if (LuaObject initialAnimation = componentTable.Get("initial_animation"); initialAnimation)
			spriteAnimationComponent.PlayAnimation(initialAnimation, true);
		
		return spriteAnimationComponent;
	}

	void SpriteAnimationComponentAddToEntity(SpriteAnimationComponent const &spriteAnimationComponent, Entity &entity)
	{
		entity.AddComponent<SpriteAnimationComponent>(spriteAnimationComponent);
	}
}

const bool SpriteAnimationComponent::mRegistered = []() {
	Reflect::Reflect<SpriteAnimationComponent>("sprite_animation")
		.AddConstructor(SpriteAnimationComponentTableCtor)
		.AddMemberFunction(SpriteAnimationComponentAddToEntity, "add_to_entity");

	return true;
}();