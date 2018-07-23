#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class Soldier;

enum MonsterType
{
	gnoll_reaver,
	gnoll_gnawer,
	gnoll_burner,
	gnoll_blighter,
	ettin,
	harraser,
	hyena,
	perython
};

enum MonsterState
{
	stateNone,
	stateWalkRight,
	stateWalkLeft,
	stateWalkUp,
	stateWalkDown,
	stateAttack,
	stateAttacked,
	stateDeath,
	stateGoal

};
enum MonsterAnimationState{
	stateWalk,
};
const Vec2 ERROR_POSITION = Vec2(-1, -1);

class Monster :public Sprite {
public:
	CC_SYNTHESIZE(float, maxHP, MaxHP);
	CC_SYNTHESIZE(float, curHP, CurHP);
	CC_SYNTHESIZE(float, runSpeed, RunSpeed);
	CC_SYNTHESIZE(int, dropMoney, DropMoney);
	CC_SYNTHESIZE(int, power, Power);
	CC_SYNTHESIZE(std::vector<Vec2>, route, Route);
	CC_SYNTHESIZE(MonsterState, state, State);
	CC_SYNTHESIZE(std::string, name, MonsterName);
	int curpos;
	//CC_SYNTHESIZE(ProgressTimer*, hpBar, HpBar);
	virtual void setType(MonsterType);
	virtual MonsterType getType();
	virtual void update(float dt);
	virtual void death();
	virtual void attack(float dt);
	Vec2 getSize();
	void getHurt();
	void walkRight();
	void walkLeft();
	void walkUp();
	void walkDown();
	void stateHandlerWalkRight(int deltax, int deltay);
	void stateHandlerWalkUp(int deltax, int deltay);
	void stateHandlerWalkLeft(int deltax, int deltay);
	void stateHandlerWalkDown(int deltax, int deltay);
	bool isReleased();
	bool isDead();
	bool isAttacked();

	void declineHp(int power);
	void setHpBar();
	void setTarget(Soldier*);
	Soldier* getTarget();
	Vec2 getNextPosition();
	Vec2 getCurrentPosition();
	Vec2 getWorldPosition();
	Vec2 getWorldPositionForecast(int step);

protected:
	MonsterType type;
	MonsterState prevState;
	Sprite* baseSprite;
	Sprite* blood;
	Soldier* target;
	
	ProgressTimer* hpBar;
	Sprite* hpBarBg;
	Animate* animateDeath;
	//bool isDeath;
	bool _isReleased;
};