#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

class DamageSystem
{
public:
	static DamageSystem &GetInstance()
	{
		static DamageSystem instance;
		return instance;
	}

	void OnCollision(struct CollisionEvent &event);
private:
	DamageSystem();
};

#endif // DAMAGESYSTEM_H
