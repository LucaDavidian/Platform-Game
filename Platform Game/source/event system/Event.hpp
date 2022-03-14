#ifndef EVENT_H
#define EVENT_H

#include <glm/glm.hpp>
#include "../ECS/Entity.h"

struct InputEvent
{
	enum class Type
	{
		KEY_PRESS, KEY_RELEASE, KEY_DOWN, MOUSE_MOVE, MOUSE_PRESS, MOUSE_RELEASE, MOUSE_DOWN,
	} mType;

	union
	{
		struct KeyPressEvent
		{
			unsigned int mKey;
		} keyPressEvent;

		struct KeyReleaseEvent
		{
			unsigned int mKey;
		} keyReleaseEvent;

		struct KeyDownEvent
		{
			unsigned int mKey;
		} keyDownEvent;

		struct MouseMoveEvent
		{
			int x;
			int y;
		} mouseMoveEvent;

		struct MousePressEvent
		{
			int x;
			int y;
		} mousePressEvent;

		struct MouseReleaseEvent
		{
			int x;
			int y;
		} mouseReleaseEvent;

		struct MouseDownEvent
		{
			int x;
			int y;
		} mouseDownEvent;
	};
};

struct CollisionEvent
{
	Entity *entity1;
	Entity *entity2;

	glm::vec2 contactPoint;
	glm::vec2 contactNormal;
	float penetration;
};

struct TransformAnimationEndEvent
{
};

struct PlayerOnPlatformEvent 
{
};

struct PlayerNotOnPlatformEvent
{
};

struct SpriteAnimationEndEvent 
{
};

struct EnemyDeadEvent
{
};

struct DamageEvent
{
	Entity *entity;
	glm::vec2 direction;
};

struct NumLivesChangedEvent
{
	unsigned int numLives;
};

struct NoLivesLeftEvent
{
};

#endif // EVENT_H
