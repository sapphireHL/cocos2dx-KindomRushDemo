#pragma once
#include "Monster.h"

class GnollReaver : public Monster {
public:
	static GnollReaver* createMonster(std::vector<Vec2> path);
	virtual bool init();
	//virtual void death();
	//virtual void update(float dt);
};