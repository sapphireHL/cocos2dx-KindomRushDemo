#pragma once
#include "Monster.h"

class GnollGnawer : public Monster {
public:
	static GnollGnawer* createMonster(std::vector<Vec2> path);
	virtual bool init();
	//virtual void death();
	//virtual void update(float dt);
};