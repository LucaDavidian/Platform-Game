#ifndef MOVETO_COMPONENT_H
#define MOVETO_COMPONENT_H

#include "../Component.h"
#include "../../data structures/vector.hpp"
#include <glm/glm.hpp>

class MoveToComponent : public Component
{
public:
	struct WayPoint
	{
		glm::vec2 start;
		glm::vec2 end;
	};
public:
	MoveToComponent(Vector<WayPoint> wayPoints) : mWayPoints(wayPoints) {}

	WayPoint GetWayPoint() const { return mWayPoints[mCurrentwayPoint]; }
	void NextWaypoint() { mCurrentwayPoint = (mCurrentwayPoint + 1) % mWayPoints.Size(); }

	float GetSpeed() const { return mSpeed; }
private:
	Vector<WayPoint> mWayPoints;
	unsigned mCurrentwayPoint = 0;

	float mSpeed = 3.0f;

	static const bool mRegistered;
};

#endif // MOVETO_COMPONENT_H
