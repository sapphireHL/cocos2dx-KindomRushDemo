#include "BarrackTower.h"
#include "GameController.h"

BarrackTower * BarrackTower::createTower(Vec2 pos, int level)
{
	auto tower = new BarrackTower;
	if (tower && tower->init(pos, level)) {
		tower->setRange(160);
		tower->setPower(5);
		tower->autorelease();
		return tower;
	}
	delete tower;
	tower = nullptr;
	return nullptr;
}

bool BarrackTower::init(Vec2 pos, int level)
{
	if (!Sprite::init())
		return false;

	this->setAnchorPoint(Vec2(0.5, 0.5));

	rallyPos = pos;
	base = Sprite::createWithSpriteFrameName("barracks_towers_layer1_0001.png");
	this->addChild(base);
	door = Sprite::createWithSpriteFrameName("barracks_towers_layer2_0001.png");
	this->addChild(door);

	//开门动画
	auto animationOpen = AnimationCache::getInstance()->getAnimation("barrack_open_door");
	auto animationClose = AnimationCache::getInstance()->getAnimation("barrack_close_door");
	open = Animate::create(animationOpen);
	close = Animate::create(animationClose);

	//创建soldiers
	soldiers.resize(3);
	for (int i = 0; i < 3; i++) {
		soldiers[i] = Soldier::createSoldier(Vec2(rallyPos.x + RelativePosition[i][0], rallyPos.y + RelativePosition[i][1]));
		soldiers[i]->setPosition(0, -30);
		soldiers[i]->setVisible(false);
		soldiers[i]->setState(Soldier::SoldierState::stateBorn);
		this->addChild(soldiers[i]);
		GameController::getInstance()->addSoldier(soldiers[i]);//加入全局
	}

	auto walk = [=]() {
		for (int i = 0; i < 3; i++) {
			soldiers[i]->setVisible(true);
			soldiers[i]->walkTo(soldiers[i]->getRallyPos());//行进到初始点

		}
	};

	auto ready = [=]() {
		for (int i = 0; i < 3; i++) {
			soldiers[i]->setState(Soldier::SoldierState::stateReady);
		}
	};
	door->runAction(Sequence::create(open, CallFunc::create(walk), DelayTime::create(0.5f), 
		close, DelayTime::create(0.5f), CallFunc::create(ready), nullptr));

	schedule(schedule_selector(BarrackTower::update), 1.0f);
	this->level = level;

	return true;
}

void BarrackTower::summonSoldiers(int num)
{

}

void BarrackTower::update(float dt)
{
	for (int i = 0; i < 3; ++i) {
		if (soldiers[i]->isDead()) {
			if (soldiers[i]->canRelife()) {
				rebirth(i);//复活士兵
				soldiers[i]->resetRelife();
			}
			else {
				soldiers[i]->countdownRelife();
			}
		}
	}
}

void BarrackTower::rebirth(int i)
{
	soldiers[i]->initSoldier();
	soldiers[i]->setVisible(false);
	auto walk = [=]() {
		soldiers[i]->setVisible(true);
		soldiers[i]->walkTo(soldiers[i]->getRallyPos());//行进到初始点
	};

	auto ready = [=]() {
		soldiers[i]->setState(Soldier::SoldierState::stateReady);
	};
	auto animationOpen = AnimationCache::getInstance()->getAnimation("barrack_open_door");
	auto animationClose = AnimationCache::getInstance()->getAnimation("barrack_close_door");
	open = Animate::create(animationOpen);
	close = Animate::create(animationClose);
	door->runAction(Sequence::create(open, CallFunc::create(walk), DelayTime::create(0.5f),
		close, DelayTime::create(0.5f), CallFunc::create(ready), nullptr));
}

void BarrackTower::setRallyPos(Vec2 pos)
{
	auto rallyWorld = convertToWorldSpace(rallyPos);
	auto delta = pos - rallyWorld;
	rallyPos = pos;
	for (int i = 0; i < 3; ++i) {
		soldiers[i]->setRallyPos(Vec2(rallyPos.x + RelativePosition[i][0], rallyPos.y + RelativePosition[i][1]));
		soldiers[i]->walkTo(soldiers[i]->getRallyPos());
	}
}

void BarrackTower::upgrade()
{
	level = 2;
	base->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("barracks_towers_layer1_0026.png"));
	for (int i = 0; i < 3; ++i) {
		soldiers[i]->upgrade();
	}
}
