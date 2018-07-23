#include "ArcherTowerLv2.h"
#include "GameController.h"
#include "Arrow.h"

ArcherTowerLv2 * ArcherTowerLv2::createTower()
{
	auto tower = new ArcherTowerLv2();
	if (tower && tower->init()) {
		tower->setType(1);
		tower->setRange(240);
		tower->setPower(5);
		tower->autorelease();
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return nullptr;
}

bool ArcherTowerLv2::init()
{
	if (!Sprite::init()) {
		return false;
	}

	base = Sprite::createWithSpriteFrameName("archer_towers_0002.png");
	shooter = Sprite::createWithSpriteFrameName("archer_shooter_0009.png");
	shooter2 = Sprite::createWithSpriteFrameName("archer_shooter_0009.png");
	shooter->setPosition(-15, 10);
	shooter2->setPosition(15, 20);
	this->addChild(base);
	this->addChild(shooter);
	this->addChild(shooter2);

	schedule(schedule_selector(ArcherTower::update), 0.1f);
	nearestMonster = nullptr;

	return true;
}

void ArcherTowerLv2::update(float dt)
{
	if (searchMonster()) {
		shoot(0.1f);
		schedule(schedule_selector(ArcherTower::update), 0.8f);
	}
	else {
		schedule(schedule_selector(ArcherTower::update), 0.1f);
	}
}

bool ArcherTowerLv2::searchMonster()
{
	nearestMonster = nullptr;
	auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
	auto myPos = convertToWorldSpace(this->getPosition());
	for (auto mons : monsters) {
		if (mons->isDead()) continue;
		auto monsterPos = mons->getWorldPosition();
		float dis = myPos.distance(monsterPos);
		if (dis < range) {
			nearestMonster = mons;
			return true;
		}
	}
	return false;
}

void ArcherTowerLv2::shoot(float dt)
{
	//创建弓箭
	auto arrow1 = Arrow::creat();
	arrow1->setFlippedX(true);
	arrow1->setPosition(shooter->getPosition());
	this->addChild(arrow1);

	auto arrow2 = Arrow::creat();
	arrow2->setFlippedX(true);
	arrow2->setPosition(shooter2->getPosition());
	this->addChild(arrow2);

	//shooter攻击动画
	auto shootAnimation = AnimationCache::getInstance()->getAnimation("archer_shooter_attack");
	shooter->runAction(Animate::create(shootAnimation));
	shooter2->runAction(Animate::create(shootAnimation));

	//攻击位置
	Vec2 shootPos;
	if (nearestMonster->isAttacked()) {
		shootPos = nearestMonster->getWorldPosition();
	}
	else
	{
		shootPos = nearestMonster->getWorldPositionForecast(1);
	}
	auto delta1 = shootPos - convertToWorldSpace(arrow1->getPosition());
	auto delta2 = shootPos - convertToWorldSpace(arrow2->getPosition());
	//调整转向
	if (delta1.x > 0) {
		shooter->setFlippedX(false);
	}
	else {
		shooter->setFlippedX(true);
	}
	if (delta2.x > 0) {
		shooter2->setFlippedX(false);
	}
	else {
		shooter2->setFlippedX(true);
	}
	//设置箭属性
	arrow1->calcRoute(delta1);
	arrow1->setTarget(nearestMonster);
	arrow1->setPower(power);
	arrow1->shoot();
	arrow2->calcRoute(delta2);
	arrow2->setTarget(nearestMonster);
	arrow2->setPower(power);
	arrow2->shoot();
}
