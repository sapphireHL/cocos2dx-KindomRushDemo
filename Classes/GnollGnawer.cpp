#include "GnollGnawer.h"

GnollGnawer * GnollGnawer::createMonster(std::vector<Vec2> path)
{
	auto ms = new GnollGnawer;
	if (ms && ms->init()) {
		ms->setMonsterName("gnoll_gnawer");
		ms->setState(MonsterState::stateWalkLeft);
		ms->setMaxHP(100);
		ms->setCurHP(100);
		ms->setRoute(path);
		ms->setDropMoney(20);
		ms->setRunSpeed(40);
		ms->setPower(10);
		ms->autorelease();
		return ms;
	}
	CC_SAFE_DELETE(ms);
	return nullptr;
}

bool GnollGnawer::init()
{
	if (!Sprite::init())
		return false;

	curpos = 0;
	_isReleased = false;
	setType(MonsterType::gnoll_gnawer);
	baseSprite = Sprite::createWithSpriteFrameName("gnoll_gnawer_0067.png");
	baseSprite->setFlippedX(true);

	//walk
	auto animation = AnimationCache::getInstance()->getAnimation("gnoll_gnawer_walkingRightLeft");
	auto animate = Animate::create(animation);
	this->addChild(baseSprite);
	auto walk = RepeatForever::create(animate);
	walk->setTag(stateWalk);
	baseSprite->runAction(RepeatForever::create(animate));

	//hpbar
	this->setHpBar();

	//blood
	blood = Sprite::createWithSpriteFrameName("fx_blood_splat_red_0001.png");
	blood->setVisible(false);
	this->addChild(blood);

	target = nullptr;

	schedule(schedule_selector(Monster::update), 0.2f);
	return true;
}
