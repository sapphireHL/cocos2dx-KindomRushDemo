#pragma once
#include "cocos2d.h"
#include "Soldier.h"

using namespace cocos2d;

class HeroArcher :public Soldier {
public:
	enum HeroState{
		stateIdleBow,
		stateWalk,
		stateAttackBow,
		stateAttackSword,
		stateAttacked,
		stateDeath
	};
	static HeroArcher* createHero();
	bool init();
	void update(float dt);
	void walkBy(Vec2 pos);
	void idle();
	bool searchMonster();
	void shootBow(float dt);
	void attackSword(float dt);
	bool isTooNear();

private:
	HeroState state;
	Vec2 birthPlace;
	Sprite* heroSelected;
	Sprite* confirm;
	int touchCnt;
};