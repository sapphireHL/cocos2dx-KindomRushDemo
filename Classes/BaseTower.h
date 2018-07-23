#pragma once
#include "cocos2d.h"

using namespace cocos2d;



class Tower :public Sprite {
public:
	virtual bool init();
	CC_SYNTHESIZE(int, type, Type);
	CC_SYNTHESIZE(double, range, Range);
	CC_SYNTHESIZE(int, power, Power);
	CC_SYNTHESIZE(int, level, Level);
	virtual void setRallyPos(Vec2) {};
	virtual void upgrade() {};
};