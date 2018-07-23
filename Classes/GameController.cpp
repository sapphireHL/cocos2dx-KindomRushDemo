#include "GameController.h"

GameController* GameController::instance;

GameController * GameController::getInstance()
{
	if (instance == nullptr)
		instance = new GameController;
	return instance;
}

void GameController::addMonster(Monster *ms)
{
	waveMonsters.push_back(ms);
}

void GameController::clearMonsters()
{
	waveMonsters.clear();
}

std::vector<Monster*> GameController::getCurrentWaveMonsters()
{
	return waveMonsters;
}

void GameController::addSoldier(Soldier * s)
{
	currentSoldiers.push_back(s);
}

void GameController::clearSoldiers()
{
	currentSoldiers.clear();
}

std::vector<Soldier*> GameController::getCurrentSoldiers()
{
	return currentSoldiers;
}

void GameController::setBg(Sprite *background)
{
	bg = background;
}

Sprite * GameController::getBg()
{
	return bg;
}

void GameController::setGameScene(Scene * scene)
{
	gameScene = scene;
}

Scene * GameController::getGameScene()
{
	return gameScene;
}
