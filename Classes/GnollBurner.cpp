#include "GnollBurner.h"

GnollBurner * GnollBurner::createMonster(std::vector<Vec2> path)
{
	auto gb = new GnollBurner;
	if (gb && gb->init()) {
		gb->setMonsterName("gnoll_burner");
		gb->setState(MonsterState::stateWalkLeft);
		gb->setMaxHP(50);
		gb->setCurHP(50);
		gb->setRoute(path);
		gb->setDropMoney(10);
		gb->setRunSpeed(40);
		gb->setPower(6);
		gb->autorelease();
		return gb;
	}
	delete gb;
	gb = nullptr;
	return nullptr;
}

bool GnollBurner::init()
{
	if (!Sprite::init()) {
		return false;
	}

	curpos = 0;
	_isReleased = false;
	setType(MonsterType::gnoll_burner);
	baseSprite = Sprite::createWithSpriteFrameName("gnoll_burner_0001.png");
	baseSprite->setFlippedX(true);
	//walk
	auto animation = AnimationCache::getInstance()->getAnimation("gnoll_burner_walkingRightLeft");
	auto animate = Animate::create(animation);
	this->addChild(baseSprite);
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

//void GnollBurner::death()
//{
//	auto animationDeath = AnimationCache::getInstance()->getAnimation("gnoll_burner_death");
//	animateDeath = Animate::create(animationDeath);
//	baseSprite->stopAllActions();
//	auto remove = [=]() {
//		baseSprite->removeFromParent();
//		hpBarBg->removeFromParent();
//		_isReleased = true;
//	};
//	baseSprite->runAction(Sequence::create(animateDeath, DelayTime::create(0.5f), CallFunc::create(remove), nullptr));
//}
