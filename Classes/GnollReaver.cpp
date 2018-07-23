#include "GnollReaver.h"

GnollReaver * GnollReaver::createMonster(std::vector<Vec2> path)
{
	auto ms = new GnollReaver;
	if (ms && ms->init()) {
		ms->setMonsterName("gnoll_reaver");
		ms->setState(MonsterState::stateWalkLeft);
		ms->setMaxHP(40);
		ms->setCurHP(40);
		ms->setRoute(path);
		ms->setDropMoney(10);
		ms->setRunSpeed(40);
		ms->setPower(3);
		ms->autorelease();
		return ms;
	}
	CC_SAFE_DELETE(ms);
	return nullptr;
}

bool GnollReaver::init()
{
	if (!Sprite::init())
		return false;

	curpos = 0;
	//isDeath = false;
	_isReleased = false;
	setType(MonsterType::gnoll_reaver);
	baseSprite = Sprite::createWithSpriteFrameName("gnoll_reaver_0066.png");
	baseSprite->setFlippedX(true);
	//walk
	auto animation = AnimationCache::getInstance()->getAnimation("gnoll_reaver_walkingRightLeft");
	auto animate = Animate::create(animation);
	this->addChild(baseSprite);
	auto walk = RepeatForever::create(animate);
	walk->setTag(stateWalk);
	baseSprite->runAction(RepeatForever::create(animate));
	//hpbar
	this->setHpBar();
	//hpBarBg->setPosition(baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height);

	//blood
	blood = Sprite::createWithSpriteFrameName("fx_blood_splat_red_0001.png");
	blood->setVisible(false);
	this->addChild(blood);

	target = nullptr;

	schedule(schedule_selector(Monster::update), 0.2f);
	return true;
}

//void GnollReaver::death()
//{
//	auto animationDeath = AnimationCache::getInstance()->getAnimation("gnoll_reaver_death");
//	animateDeath = Animate::create(animationDeath);
//	baseSprite->stopAllActions();
//	auto remove = [=]() {
//		baseSprite->removeFromParent();
//		hpBarBg->removeFromParent();
//		_isReleased = true;
//		//this->removeFromParent();
//		//_isReleased = true;
//		//delete this;
//		//this = nullptr;
//		//baseSprite->setVisible(false);
//	};
//	baseSprite->runAction(Sequence::create(animateDeath, DelayTime::create(0.5f), CallFunc::create(remove), nullptr));
//	//this->removeFromParent();
//}

//void GnollReaver::update(float dt)
//{
//	auto animation = AnimationCache::getInstance()->getAnimation("gnoll_reaver_walkingRightLeft");
//	auto animate = Animate::create(animation);
//	baseSprite->runAction(RepeatForever::create(animate));
//	
//	if (++curpos >= route.size()) {
//		return;
//	}
//	auto currpos1 = this->getPosition();
//	auto currpos2 = convertToNodeSpace(currpos1);
//	auto currpos3 = convertToWorldSpace(currpos1);
//	auto currpos4 = this->convertToWorldSpace(currpos1);
//	auto delta = route[curpos] - this->getPosition();
//	//auto nextpos = baseSprite->convertToNodeSpace(route[curpos]);
//	auto moveby = MoveBy::create(dt, delta);
//	baseSprite->runAction(moveby);
//	//baseSprite->setPosition(route[curpos]);
//}
