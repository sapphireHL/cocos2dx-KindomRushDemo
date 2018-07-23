#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "BuildTerrain.h"
#include "TimeEvent.h"

using namespace cocos2d;

class SkillItem;

class GameScene : public Scene {
public:
	const int WAVE_TIME[6] = { 0, 30, 60, 107, 145, 181 };
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

	void initTerrains();
	void addMonsters();
	void addOneMonster(MonsterType type, int road, int pathnumber);
	void update(float dt) override;
	void monsterUpdate(float dt);
	void towerUpdate(float dt);
	void timerUpdate(float dt);
	void gameTimer(float dt);

	std::vector<Vec2> correctRoute(float x, float y, std::vector<Vec2> route);

public:
	TimeEvent events;
	std::vector<std::vector<std::vector<Vec2>>> path;
	std::vector<BuildTerrain*> terrains;
	std::vector<Monster*> monsters;
	int curpos;
	int touchCnt;
	int totalTime;
	int curWave;
	Sprite* bg;
	Sprite* waveFlag;
	Sprite* waveFlagSelected;
	ProgressTimer* waveTimer;
	Sprite* ms;
	Monster* gr, *gr2, *gr3, *gr4, *gr5, *gr6;
	BuildTerrain* terrain1, *terrain2, *terrain3, *terrain4, *terrain5, *terrain6, *terrain7, *terrain8;
	Label* label;
	SkillItem* skill;
};