#pragma once
#include "cocos2d.h"
#include "BaseTower.h"
#include "Bullet.h"
#include "Monster.h"
using namespace cocos2d;

const int LEFT = 1;//0001
const int RIGHT = 2;//0010
const int UP = 4;//0100
const int DOWN = 8;//1000
const int LEFTUP = 10;
const int LEFTDOWN = 11;
const int RIGHTUP = 12;
const int RIGHTDOWN = 13;
class ArcherTower :public Tower {
public:
	static ArcherTower* createTower(int level = 1);
	bool init(int level);
	void shoot();
	void update(float dt);
	bool searchMonster();
	bool hitOnMonster();
	Vec2 correctShoot(MonsterState st);

protected:
	Sprite* base;
	Sprite* shooter;
	Bullet* arrow;
	Vec2 shootPos;
	Monster* nearestMonster;
	int monsterOrientation;
};