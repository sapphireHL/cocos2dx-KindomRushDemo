#pragma once
#include "cocos2d.h"
#include "Bullet.h"

using namespace cocos2d;

class Arrow :public Bullet {
public:
	static Arrow* creat();
	bool init();
	void shoot();
	void setFlip();
	void setRotation(float);
	void calcRoute(Vec2 delta);
};