#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "Soldier.h"

using namespace cocos2d;

class Monster;
class Soldier;

class GameController {
public:
	static GameController* getInstance();
	void addMonster(Monster*);
	void clearMonsters();
	std::vector<Monster*> getCurrentWaveMonsters();
	void addSoldier(Soldier*);
	void clearSoldiers();
	std::vector<Soldier*> getCurrentSoldiers();
	void setBg(Sprite*);
	Sprite* getBg();
	void setGameScene(Scene*);
	Scene* getGameScene();
private:
	GameController() {};
	GameController(const GameController&) {};
	GameController& operator=(const GameController&) {};


private:
	static GameController* instance;
	std::vector<Monster*> waveMonsters;
	std::vector<Soldier*> currentSoldiers;
	Sprite* bg;
	Scene* gameScene;
};