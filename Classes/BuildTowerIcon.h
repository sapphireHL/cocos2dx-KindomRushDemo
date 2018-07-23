#pragma once
#include "cocos2d.h"

using namespace cocos2d;

const int ICON_TYPE_ARCHER = 1;
const int ICON_TYPE_BARRACKS = 2;
const int ICON_TYPE_MAGE = 3;
const int ICON_TYPE_ARTILLERY = 4;
const int ICON_TYPE_UPGRADE = 5;
const int ICON_TYPE_SELL = 6;
const int ICON_TYPE_RERALLY = 7;

class BuildTowerIcon :public Sprite {
public:
	static BuildTowerIcon* createIcon(int type);
	virtual bool init(int type);
	bool isSelected();
	bool isDoubleSelected();

public:

protected:
	Sprite* icon;
	Sprite* preview;
	Sprite* tower;
	bool isTouched;
	bool isBuildReady;
};