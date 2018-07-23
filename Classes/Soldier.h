#pragma once
#include "cocos2d.h"
//#include "GameController.h"

using namespace cocos2d;

class Monster;

class Soldier :public Sprite {
public:
	enum SoldierState {
		stateBorn,
		stateReady,
		stateWalk,
		stateAttack,
		stateAttacked,
		stateDeath
	};
	static Soldier* createSoldier(Vec2 pos);
	bool init(Vec2 pos);
	CC_SYNTHESIZE(float, maxHP, MaxHP);
	CC_SYNTHESIZE(float, curHP, CurHP);
	CC_SYNTHESIZE(Vec2, rallyPos, RallyPos);
	CC_SYNTHESIZE(int, power, Power);
	CC_SYNTHESIZE(SoldierState, state, State);
	virtual void walkTo(Vec2);
	virtual void walkBy(Vec2);
	virtual void attack(float dt);
	virtual void death();
	virtual void setHpBar();
	virtual void update(float dt);
	virtual void upgrade();
	bool searchMonster();
	Vec2 getPosition();
	void setPositionInBg(Vec2 pos);
	Vec2 getPositionInBg();
	Vec2 getWorldPosition();

	void declineHp(int power);
	bool isDead();
	bool canRelife();
	void resetRelife();
	void countdownRelife();
	void setSoldierPosition(Vec2);
	void setTarget(Monster*);
	void initSoldier();

protected:
	Sprite* base;
	Sprite* blood;
	Vec2 positionInBg;

	ProgressTimer* hpBar;
	Sprite* hpBarBg;
	Animate* animateWalk;
	Animate* animateAttack;
	Animate* animateDeath;
	Monster* target;
	bool isDeath;
	bool isReleased;
	int relifeCount;
	int level;
};