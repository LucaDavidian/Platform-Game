#ifndef SPRITEANIMATION_SYSTEM_H
#define SPRITEANIMATION_SYSTEM_H

class SpriteAnimationSystem
{
public:
	static SpriteAnimationSystem &GetInstance() 
	{ 
		static SpriteAnimationSystem instance; 
		return instance; 
	}

	void Update(float dt);
private:
	SpriteAnimationSystem() = default;
};

#endif  // SPRITEANIMATION_SYSTEM_H

