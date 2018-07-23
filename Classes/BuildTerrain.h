#pragma once
#include "cocos2d.h"
#include "BuildTowerIcon.h"
#include "BaseTower.h"

using namespace cocos2d;



class BuildTerrain : public Sprite {
public:
	static BuildTerrain* createTerrain(Vec2);
	virtual bool init();
	bool callback_touch_began(Touch* touch, Event* event);
	void callback_touch_ended(Touch* touch, Event* event);
	void showMenu();
	void hideMenu();
	bool isIconSelected();
	bool isBuildReady();
	void buildTower();
	virtual void update(float dt) override;
	void initRange();

private:
	Sprite* terrain;
	//TowerPanel* buildPanel;
	Sprite* buildPanel;
	Sprite* upgradePanel;
	Sprite* preview;
	Sprite* range;
	Sprite* rallyRange;
	//Sprite* tower;
	BuildTowerIcon* icons[4];
	BuildTowerIcon* upgradeIcon;
	BuildTowerIcon* sellIcon;
	BuildTowerIcon* reRallyIcon;
	bool isMenuShown;
	bool isBuilt;
	bool resetRallyReady;
	int buildType;
	int selectedType;
	int towerType;
	Tower* tower;
	Vec2 rallyPos;
};