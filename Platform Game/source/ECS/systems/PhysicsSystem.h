#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

class PhysicsSystem
{
public:
	static PhysicsSystem &GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}

	void Update(float dt);
private:
	PhysicsSystem() = default;

	float mGravity = 1.0f;
};

#endif // PHYSICS_SYSTEM_H
