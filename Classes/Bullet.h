#pragma once
#include "cocos2d.h"
#include "Monster.h"

using namespace cocos2d;

class Bullet :public Sprite {
public:
	static Bullet* create();
	bool init();
	void setAction(Spawn*);
	virtual void shoot() {};
	virtual void setFlip();
	Vec2 getBulletPosition();
	void setTarget(Monster* t);
	void setPower(int pow);

protected:
	Sprite* bulletSprite;
	Spawn* action;
	Monster* target;
	int power;
};