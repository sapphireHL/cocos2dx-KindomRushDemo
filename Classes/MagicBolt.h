#pragma once
#include "cocos2d.h"
#include "Bullet.h"

using namespace cocos2d;

class MagicBolt :public Bullet {
public:
	static MagicBolt* creat();
	bool init();
	void shoot();
	void setTargetPos(Vec2 pos);
	void setMove(ActionInterval* act);

private:
	Vec2 targetPos;
	MotionStreak* particle;
	ActionInterval* move;
};