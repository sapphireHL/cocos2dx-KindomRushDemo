#pragma once
#include "Monster.h"

class GnollBurner : public Monster {
public:
	static GnollBurner* createMonster(std::vector<Vec2> path);
	virtual bool init();
	//virtual void death();
};