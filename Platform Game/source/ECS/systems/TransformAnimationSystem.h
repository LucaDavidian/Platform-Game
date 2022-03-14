#ifndef TRANSFORMANIMATIONSYSTEM_H
#define TRANSFORMANIMATIONSYSTEM_H

class TransformAnimationSystem
{
public:
	static TransformAnimationSystem &GetInstance()
	{
		static TransformAnimationSystem instance;
		return instance;
	}

	void Update(float dt);
private:
	TransformAnimationSystem() = default;
};

#endif // TRANSFORMANIMATIONSYSTEM_H