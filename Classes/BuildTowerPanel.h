#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class BuildTowerPanel : public Sprite {
public:
	static BuildTowerPanel* createPanel();
	virtual bool init();
	void inAnimation();
	void outAnimation();

private:
	Sprite* ring;

};