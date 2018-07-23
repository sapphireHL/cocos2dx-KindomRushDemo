#pragma once
#include "cocos2d.h"
#include "BaseTower.h"
#include "Bullet.h"
#include "Monster.h"
using namespace cocos2d;

class ArtilleryTower :public Tower {
public:
	static ArtilleryTower* createTower(int level = 1);
	bool init(int level);
	void shoot();
	void update(float dt);
	bool searchMonster();

private:
	Sprite* base;
	Sprite* thrower;
	Vec2 shootPos;
};