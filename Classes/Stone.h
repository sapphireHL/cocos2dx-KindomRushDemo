#pragma once
#include "cocos2d.h"
#include "Bullet.h"

using namespace cocos2d;

class Stone :public Bullet {
public:
	static Stone* creat();
	bool init();
	void shoot();
	void setRange(float ran);
	void setTargetPos(Vec2 pos);

private:
	float range;
	Vec2 targetPos;
};