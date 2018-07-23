#pragma once
#include "cocos2d.h"
#include "BaseTower.h"
#include "Soldier.h"

using namespace cocos2d;

const int RelativePosition[3][2] = { { 0,15 },{ -15,-15 },{ 15,-15 } };//3个塔对于召集点的位置
class BarrackTower :public Tower {
public:
	static BarrackTower* createTower(Vec2 pos, int level = 1);
	bool init(Vec2 pos, int level);
	void summonSoldiers(int num = 3);
	virtual void update(float dt);
	virtual void rebirth(int i);
	void setRallyPos(Vec2);
	void upgrade();

private:
	Sprite* base;
	Sprite* door;
	Animate* open, *close;
	Vec2 rallyPos;
	std::vector<Soldier*> soldiers;
};