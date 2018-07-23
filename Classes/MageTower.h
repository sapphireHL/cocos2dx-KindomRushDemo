#pragma once
#include "cocos2d.h"
#include "BaseTower.h"
using namespace cocos2d;

class Monster;
class MageTower :public Tower {
public:
	static MageTower* createTower(int level = 1);
	bool init(int level);
	void shoot(bool down);
	void update(float dt);
	bool searchMonster();
	void upgrade();

private:
	Sprite* terrain;
	Sprite* base;
	Sprite* magician;
	std::vector<Monster*> nearMonsters;
};