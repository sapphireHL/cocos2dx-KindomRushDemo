#pragma once
#include "SkillItem.h"

using namespace cocos2d;

class SkillLightening :public SkillItem {
public:
	static SkillLightening* createSkillItem();
	bool init();
	void applySkill(Vec2 pos);
	void cool(float dt);
private:

};