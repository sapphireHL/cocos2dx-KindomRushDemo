#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class SkillItem :public Sprite {
public:
	virtual bool init();
	virtual void applySkill(Vec2 pos);
	virtual void cool(float dt);
	void select();
	void unselect();
	bool selected();
	
protected:
	Sprite* item;
	Sprite* itemSelected;
	Sprite* itemCool;
	ProgressTimer* coolTimer;
	float coolTime;
	bool isCool;
};