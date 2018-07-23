#include "GameScene.h"
#include "BuildTerrain.h"
#include "AnimationManager.h"
#include <fstream>
#include <string>
#include "GnollReaver.h"
#include "GnollBurner.h"
#include "GnollGnawer.h"
#include "GameController.h"
#include "HeroArcher.h"
#include "SkillLightening.h"

bool getLineWithoutSpace(std::string &s, std::ifstream& in);
float getPos(std::string s);

Scene * GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
		return false;

	GameController::getInstance()->setGameScene(this);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto center = Size(visibleSize.width / 2, visibleSize.height / 2);

	touchCnt = 0;
	totalTime = -1;
	curWave = 0;
	//wave信息
	events.readEvents("level1.csv");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("woods_content-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("elves_towers-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("elves_towers_2-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("elves_woods_2_enemies-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common_spritesheet_16_na-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ingame_gui-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("elves_hero_archer-hd.plist");

	AnimationManager::getInstance()->init_woods();
	AnimationManager::getInstance()->init_archer();
	AnimationManager::getInstance()->init_barrack();
	AnimationManager::getInstance()->init_artillery();
	AnimationManager::getInstance()->init_mage();
	AnimationManager::getInstance()->init_hero_archer();
	AnimationManager::getInstance()->init_skill();

	auto mp = FileUtils::getInstance()->getValueVectorFromFile("level1_paths.plist");
	//auto paths = mp.at("path").asValueMap();
	//for()

	//背景
	bg = Sprite::createWithSpriteFrameName("Stage_1.png");
	bg->setPosition(center);
	this->addChild(bg);
	GameController::getInstance()->setBg(bg);
	
	//std::string time = String::createWithFormat("%d", totalTime)->getCString();
	//label = Label::createWithTTF(time, "fonts/arial.ttf", 50);
	//label->setPosition(300, 300);
	//this->addChild(label);

	//wave警告
	auto waveBack = Sprite::createWithSpriteFrameName("waveFlag_0003.png");
	waveTimer = ProgressTimer::create(waveBack);
	waveTimer->setPosition(1170, bg->getContentSize().height - 218);
	waveTimer->setPercentage(100);
	waveTimer->setType(ProgressTimerType::RADIAL);
	bg->addChild(waveTimer);
	waveFlag = Sprite::createWithSpriteFrameName("waveFlag_0001.png");
	waveFlag->setPosition(1170, bg->getContentSize().height - 218);
	bg->addChild(waveFlag);
	//wave选中
	waveFlagSelected = Sprite::createWithSpriteFrameName("waveFlag_selected.png");
	waveFlagSelected->setPosition(1170, bg->getContentSize().height - 218);
	waveFlagSelected->setVisible(false);
	bg->addChild(waveFlagSelected);

	//最开始的wave touch
	auto waveTouch = EventListenerTouchOneByOne::create();
	waveTouch->onTouchBegan = [=](Touch* touch, Event* event) {
		return true;
	};
	waveTouch->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = dynamic_cast<Sprite*>(event->getCurrentTarget());
		auto pos = target->convertTouchToNodeSpace(touch);
		auto rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
		if (rect.containsPoint(pos)) {
			if (touchCnt == 1) {
				//schedule(schedule_selector(GameScene::monsterUpdate), 0.2f);
				schedule(schedule_selector(GameScene::towerUpdate), 0.6f);
				schedule(schedule_selector(GameScene::gameTimer), 1.0f);
				//schedule(schedule_selector(GameScene::timerUpdate), 0.05f);
				unschedule(schedule_selector(GameScene::timerUpdate));
				waveFlag->setVisible(false);
				waveTimer->setVisible(false);
				waveFlagSelected->setVisible(false);
				//totalTime = WAVE_TIME[curWave] - 1;
				curWave++;
				touchCnt = 0;
			}
			else {
				touchCnt = 1;
				waveFlagSelected->setVisible(true);
			}
		}
		else {
			waveFlagSelected->setVisible(false);
			touchCnt = 0;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(waveTouch, waveFlag);

	//加塔点
	initTerrains();

	//加英雄
	auto hero = HeroArcher::createHero();
	bg->addChild(hero, 10);

	//拖动地图
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesMoved = [=](std::vector<Touch*> touches, Event* event) {
		if (touches.size() == 1) {
			auto touch = touches[0];
			auto diff = touch->getDelta();
			auto curpos = bg->getPosition();
			auto newpos = curpos + diff;
			auto bgsize = bg->getContentSize();
			//x,y约束，控制边界
			newpos.x = std::min(newpos.x, center.width + (bgsize.width - visibleSize.width) / 2);
			newpos.x = std::max(newpos.x, center.width - (bgsize.width - visibleSize.width) / 2);
			newpos.y = std::min(newpos.y, center.height + (bgsize.height - visibleSize.height) / 2);
			newpos.y = std::max(newpos.y, center.height - (bgsize.height - visibleSize.height) / 2);
			bg->setPosition(newpos);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, bg);



	//解析路径
	std::ifstream in("level1_paths.plist");
	std::string s;

	std::vector<Vec2> tmp1;
	std::vector<std::vector<Vec2>> tmp2;

	while (getLineWithoutSpace(s, in)) {
		if (s == "{") {

			while (getLineWithoutSpace(s, in)) {
				if (s == ");") break;
				if (s == "subpaths = (") {
					tmp2.clear();
					while (getLineWithoutSpace(s, in)) {
						if (s == ")" || s == ");")break;
						if (s == "(" || s == ",(") {
							tmp1.clear();
							while (getLineWithoutSpace(s, in)) {
								if (s == ")") break;
								getLineWithoutSpace(s, in);
								float x = getPos(s);
								getLineWithoutSpace(s, in);
								float y = getPos(s);
								getLineWithoutSpace(s, in);
								tmp1.push_back(Vec2(x, y));
							}
							tmp2.push_back(tmp1);
						}

					}
					if (!tmp2.empty()) path.push_back(tmp2);
				}

			}
			//path.push_back(tmp2);
		}
	}
	in.close();
	double offset_x = 50, offset_y = 40;
	double scale_x = 1.15, scale_y = 1.2;
	//画出路线
	//for (int i = 0; i < path[0].size(); i++) {
	//	for (int j = 0; j < path[0][i].size(); j++) {
	//		auto label = Label::createWithTTF(String::createWithFormat("%d", j)->getCString(), "fonts/Comic Book Italic.ttf", 10);
	//		label->setColor(Color3B::BLACK);
	//		label->setPosition(scale_x * path[0][i][j].x , scale_y * path[0][i][j].y + offset_y);
	//		bg->addChild(label);
	//	}
	//}
	//for (int i = 0; i < path[1].size(); i++) {
	//	for (int j = 0; j < path[1][i].size(); j++) {
	//		auto label = Label::createWithTTF(String::createWithFormat("%d", j)->getCString(), "fonts/Comic Book Italic.ttf", 10);
	//		label->setColor(Color3B::RED);
	//		label->setPosition(scale_x *path[1][i][j].x, scale_y *  path[1][i][j].y + offset_y);
	//		bg->addChild(label);
	//	}
	//}
	for (int i = 0; i < (int)path.size(); i++) {
		for (int j = 0; j < (int)path[i].size(); j++) {
			for (int k = 0; k < (int)path[i][j].size(); k++) {
				path[i][j][k].x = scale_x *path[i][j][k].x;
				path[i][j][k].y = scale_y *  path[i][j][k].y + offset_y;
			}
		}
	}




	

	//ms = Sprite::createWithSpriteFrameName("gnoll_reaver_0066.png");
	//ms->setPosition(path[0][0][0]);
	//curpos = 0;
	//auto animation = AnimationCache::getInstance()->getAnimation("gnoll_reaver_walkingRightLeft");
	//auto animate = Animate::create(animation);
	//ms->runAction(RepeatForever::create(animate));
	//ms->setFlippedX(true);
	//bg->addChild(ms);

	//schedule(schedule_selector(GameScene::monsterUpdate), 0.2f);
	//schedule(schedule_selector(GameScene::towerUpdate), 0.6f);
	//schedule(schedule_selector(GameScene::timerUpdate), 0.05f);

	//技能图标
	skill = SkillLightening::createSkillItem();
	skill->setPosition(1050, 75);
	this->addChild(skill);

	
	return true;
}

void GameScene::initTerrains()
{
	terrains.resize(8);
	terrains[0] = BuildTerrain::createTerrain(Vec2(50, 80));//这个参数是建造士兵塔时的出生地点，由于和地形有关，故手动传入,相对位置
	terrains[0]->setPosition(726, bg->getContentSize().height - 705);
	bg->addChild(terrains[0], 5);
	terrains[1] = BuildTerrain::createTerrain(Vec2(-50, 80));
	terrains[1]->setPosition(986, bg->getContentSize().height - 303);
	bg->addChild(terrains[1], 5);
	terrains[2] = BuildTerrain::createTerrain(Vec2(-100, 50));
	terrains[2]->setPosition(911, bg->getContentSize().height - 607);
	bg->addChild(terrains[2], 5);
	terrains[3] = BuildTerrain::createTerrain(Vec2(0, -80));
	terrains[3]->setPosition(998, bg->getContentSize().height - 690);
	bg->addChild(terrains[3], 5);
	terrains[4] = BuildTerrain::createTerrain(Vec2(0, -60));
	terrains[4]->setPosition(705, bg->getContentSize().height - 540);
	bg->addChild(terrains[4], 5);
	terrains[5] = BuildTerrain::createTerrain(Vec2(-10, -80));
	terrains[5]->setPosition(517, bg->getContentSize().height - 539);
	bg->addChild(terrains[5], 5);
	terrains[6] = BuildTerrain::createTerrain(Vec2(50, 80));
	terrains[6]->setPosition(333, bg->getContentSize().height - 628);
	bg->addChild(terrains[6], 5);
	terrains[7] = BuildTerrain::createTerrain(Vec2(0, 100));
	terrains[7]->setPosition(212, bg->getContentSize().height - 554);
	bg->addChild(terrains[7], 5);
}

void GameScene::addMonsters()
{
	monsters.resize(6);
	monsters[0] = GnollReaver::createMonster(path[0][0]);
	monsters[0]->setPosition(path[0][0][0]);	
	bg->addChild(monsters[0]);
	monsters[1] = GnollReaver::createMonster(correctRoute(10, 0, path[0][1]));
	monsters[1]->setPosition(monsters[1]->getCurrentPosition());
	bg->addChild(monsters[1]);
	monsters[2] = GnollReaver::createMonster(correctRoute(20, 0, path[0][2]));
	monsters[2]->setPosition(monsters[2]->getCurrentPosition());
	bg->addChild(monsters[2]);
	monsters[3] = GnollReaver::createMonster(path[1][0]);
	monsters[3]->setPosition(path[1][0][0]);
	bg->addChild(monsters[3]);
	monsters[4] = GnollReaver::createMonster(correctRoute(10, 0, path[1][1]));
	monsters[4]->setPosition(monsters[4]->getCurrentPosition());
	bg->addChild(monsters[4]);
	monsters[5] = GnollReaver::createMonster(correctRoute(20, 0, path[1][2]));
	monsters[5]->setPosition(monsters[5]->getCurrentPosition());
	bg->addChild(monsters[5]);

	//加入全局，方便被塔遍历
	GameController::getInstance()->addMonster(gr);
	GameController::getInstance()->addMonster(gr2);
	GameController::getInstance()->addMonster(gr3);
	GameController::getInstance()->addMonster(gr4);
	GameController::getInstance()->addMonster(gr5);
	GameController::getInstance()->addMonster(gr6);
}

void GameScene::addOneMonster(MonsterType type, int road, int pathnumber)
{
	Monster* ms;
	switch (type)
	{
	case gnoll_reaver:
		ms = GnollReaver::createMonster(path[road][pathnumber]);
		break;
	case gnoll_gnawer:
		ms = GnollGnawer::createMonster(path[road][pathnumber]);
		break;
	case gnoll_burner:
		ms = GnollBurner::createMonster(path[road][pathnumber]);
		break;
	case gnoll_blighter:
		break;
	case ettin:
		break;
	case harraser:
		break;
	case hyena:
		break;
	case perython:
		break;
	default:
		break;
	}
	ms->setPosition(path[road][pathnumber][0]);
	bg->addChild(ms);
	monsters.push_back(ms);
	GameController::getInstance()->addMonster(ms);
}

void GameScene::update(float dt)
{
	gr->update(dt);
	terrain1->update(dt);
	terrain2->update(dt);
	
	//ms->update(dt);
	//if (++curpos >= path[0][0].size()) {
	//	return;
	//}
	//auto moveto = MoveTo::create(dt, path[0][0][curpos]);
	//ms->runAction(moveto);
	//ms->setPosition(path[0][0][curpos]);
}

void GameScene::monsterUpdate(float dt)
{
	auto draw = DrawNode::create();
	this->addChild(draw);
		
	for (int i = 0; i < (int)monsters.size(); i++) {
		if (!monsters[i]->isReleased()) {
			monsters[i]->update(dt);
			//auto deltax = bg->getContentSize().width/2 - bg->getPosition().x;
			//auto deltay = bg->getContentSize().height/2 - bg->getPosition().y;
			//auto pos = monsters[i]->getPosition() - Vec2(deltax, deltay);
			//draw->drawDot(convertToWorldSpace(pos), 5, Color4F::BLACK);
		}
	}
	//if(!gr->isReleased())
	//	gr->update(dt);
	//if (!gr2->isReleased())
	//	gr2->update(dt);
	//if (!gr3->isReleased())
	//	gr3->update(dt);
	//if (!gr4->isReleased())
	//	gr4->update(dt);
	//if (!gr5->isReleased())
	//	gr5->update(dt);
	//if (!gr6->isReleased())
	//	gr6->update(dt);
}

void GameScene::towerUpdate(float dt)
{
	for (int i = 0; i < 8; i++) {
		terrains[i]->update(dt);
	}
}

void GameScene::timerUpdate(float dt)
{
	float curPer = waveTimer->getPercentage();
	if (curPer == 0) {
		waveFlag->setVisible(false);
		waveTimer->setVisible(false);
		waveFlagSelected->setVisible(false);
		curWave++;
		schedule(schedule_selector(GameScene::gameTimer), 1.0f);
		unschedule(schedule_selector(GameScene::timerUpdate));
		//schedule(schedule_selector(GameScene::timerUpdate), 1.0f);
	}
	waveTimer->setPercentage(curPer - 0.1);
}

void GameScene::gameTimer(float dt)
{
	//label->removeFromParent();
	//std::string time = String::createWithFormat("%d", totalTime)->getCString();
	//label = Label::createWithTTF(time, "fonts/arial.ttf", 50);
	//label->setPosition(300, 300);
	//this->addChild(label);

	//所有事件处理完
	if (events.iaAllEventsOver()) {
		return;
	}
	totalTime++;
	log("cur time:%d\n", totalTime);
	//获取当前事件
	auto curevent = events.getCurrentEvent();
	//若时间未到
	if (totalTime < curevent.time)
		return;
	//若事件时间小，向前进
	//while (curevent.time < totalTime)
	//{
	//	events.goToNextTime();
	//	curevent = events.getCurrentEvent();
	//}
	//时间到了
	while (curevent.time == totalTime) {
		//添加怪兽
		if (curevent.type == eventType::eventAddMonster) {
			addOneMonster(curevent.monsterType, curevent.path, curevent.subpath);
		}
		//显示下一波提示
		else if (curevent.type == eventType::eventWaveAlert) {
			waveFlag->setVisible(true);
			waveTimer->setVisible(true);
			waveTimer->setPercentage(100);
			float waveTime = (float)curevent.waitTime / 1000;
			schedule(schedule_selector(GameScene::timerUpdate), waveTime);
			unschedule(schedule_selector(GameScene::gameTimer));
		}
		//获取下一个事件，若时间相同，在循环内一并处理
		events.goToNextTime();
		curevent = events.getCurrentEvent();
	}
	//wave1
	//if (totalTime <= 7) {
	//	int road = 0, pathnumber = random() % 3;
	//	auto ms = GnollReaver::createMonster(path[road][pathnumber]);
	//	ms->setPosition(path[road][pathnumber][0]);
	//	bg->addChild(ms);
	//	monsters.push_back(ms);
	//	GameController::getInstance()->addMonster(ms);
	//}
	//else if (totalTime >= 10 && totalTime <= 15) {
	//	int road = 0, pathnumber = random() % 3;
	//	auto ms = GnollBurner::createMonster(path[road][pathnumber]);
	//	ms->setPosition(path[road][pathnumber][0]);
	//	bg->addChild(ms);
	//	monsters.push_back(ms);
	//	GameController::getInstance()->addMonster(ms);
	//}
	////wave2 allert
	//else if (totalTime == 20) {
	//	waveFlag->setVisible(true);
	//	waveTimer->setVisible(true);
	//	schedule(schedule_selector(GameScene::timerUpdate), 0.01f);
	//}
	//else if (totalTime >= 30 && totalTime <= 33) {
	//	int road = 0, pathnumber = random() % 3;
	//	auto ms = GnollBurner::createMonster(path[road][pathnumber]);
	//	ms->setPosition(path[road][pathnumber][0]);
	//	bg->addChild(ms);
	//	monsters.push_back(ms);
	//	GameController::getInstance()->addMonster(ms);
	//}
}

std::vector<Vec2> GameScene::correctRoute(float x, float y, std::vector<Vec2> route)
{
	std::vector<Vec2> res(route.size());
	for (int i = 0; i < (int)route.size(); i++) {
		res[i] = route[i] + Vec2(x, y);
	}
	return res;
}




bool getLineWithoutSpace(std::string & s, std::ifstream & in)
{
	if (!getline(in, s))return false;
	for (int i = 0; i < (int)s.size(); i++) {
		if (s[i] == '\t') {
			s.erase(i, 1);
			i--;
		}
	}
	return true;
}

float getPos(std::string s)
{
	int pos1 = s.find('\"');
	int pos2 = s.find('\"', pos1);
	return std::stof(s.substr(pos1 + 1, pos2 - pos1 - 1));
}
