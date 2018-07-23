#pragma once
#include "ArcherTower.h"

using namespace cocos2d;

class ArcherTowerLv2 :public ArcherTower {
public:
	static ArcherTowerLv2* createTower();
	bool init();
	void update(float dt);
	bool searchMonster();
	void shoot(float dt);

private:
	Sprite* shooter2;
};