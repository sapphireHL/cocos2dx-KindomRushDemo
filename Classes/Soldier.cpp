#include "Soldier.h"
#include "GameController.h"

Soldier * Soldier::createSoldier(Vec2 pos)
{
	auto sol = new Soldier;
	if (sol && sol->init(pos)) {
		sol->autorelease();
		return sol;
	}
	delete sol;
	sol = nullptr;
	return nullptr;
}

bool Soldier::init(Vec2 pos)
{
	if (!Sprite::init()) {
		return false;
	}

	isDeath = false;
	isReleased = false;

	base = Sprite::createWithSpriteFrameName("soldiers_123_0001.png");
	this->addChild(base);
	setHpBar();

	setRallyPos(pos);
	setMaxHP(20);
	setCurHP(20);
	setPower(5);

	target = nullptr;
	relifeCount = 10;
	level = 1;

	schedule(schedule_selector(Soldier::update), 0.2f);

	return true;
}

void Soldier::setHpBar()
{
	hpBarBg = Sprite::createWithSpriteFrameName("lifebar_bg_small.png");
	auto hpBarSprite = Sprite::createWithSpriteFrameName("lifebar_small.png");
	hpBar = ProgressTimer::create(hpBarSprite);
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setPercentage(100);
	hpBar->setMidpoint(Vec2(0, 0.5));
	hpBar->setBarChangeRate(Vec2(1, 0));
	hpBar->setPosition(hpBarBg->getContentSize().width / 2, hpBarBg->getContentSize().height / 2);
	hpBarBg->addChild(hpBar);
	hpBarBg->setPosition(base->getContentSize().width / 2, base->getContentSize().height / 2 + 15);
	base->addChild(hpBarBg);
}

void Soldier::update(float dt)
{
	if (state == stateDeath || state == stateBorn)
		return;
	//»ØÑª
	if (state == stateReady) {
		if (curHP < maxHP) {
			curHP += maxHP/5;
			curHP = curHP > maxHP ? maxHP : curHP;
		}
	}
	//¸üÐÂÑªÌõ
	hpBar->setPercentage(float(100 * curHP / maxHP));
	if (curHP <= 0) {
		state = stateDeath;
		unschedule(schedule_selector(Soldier::attack));
		death();
		return;
	}

	auto myPos = convertToWorldSpace(this->getPosition());
	//ÒÑ¾­Ëø¶¨µÐÈË
	if (target) {
		//ÀëµÃÌ«Ô¶
		auto targetPos = target->getWorldPosition();
		if (myPos.distance(targetPos) > 100) {
			target = nullptr;
			unschedule(schedule_selector(Soldier::attack));//¹¥»÷Í£Ö¹
		}
		//¹¥»÷·¶Î§ÄÚ
		else {
			//attack(0.8f);
		}
	}
	//»¹Î´Ëø¶¨µÐÈË
	//ËÑË÷¹ÖÊÞ
	else {
		if (searchMonster()) {
			stopAllActions();
			auto monsterPos = target->getWorldPosition();
			walkBy(0.75*(monsterPos - myPos));
			target->setState(MonsterState::stateAttacked);
			schedule(schedule_selector(Soldier::attack), 0.8f);
		}
		//ÈôÎÞµÐÈË
		else {

		}
	}
}

void Soldier::upgrade()
{
	level = 2;
	base->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0032.png"));
	state = stateReady;
	setMaxHP(30);
	setCurHP(30);
	setPower(8);
}

bool Soldier::searchMonster()
{
	target = nullptr;
	auto myPos = convertToWorldSpace(this->getPosition());
	auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
	for (int i = 0; i < monsters.size(); ++i) {
		if (monsters[i]->isDead() || monsters[i]->isAttacked())
			continue;
		auto monsterPos = monsters[i]->getWorldPosition();
		float dis = myPos.distance(monsterPos);
		if (dis < 100) {
			target = monsters[i];
			monsters[i]->setTarget(this);
			return true;
		}
	}
	return false;
}

Vec2 Soldier::getPosition()
{
	return base->getPosition();
}

void Soldier::setPositionInBg(Vec2 pos)
{
	positionInBg = pos;
}

Vec2 Soldier::getPositionInBg()
{
	return convertToWorldSpace(this->getPosition());
}

Vec2 Soldier::getWorldPosition()
{
	return convertToWorldSpace(this->getPosition());
}

void Soldier::declineHp(int power)
{
	setCurHP(curHP - power);
}

bool Soldier::isDead()
{
	return state == stateDeath;
}

bool Soldier::canRelife()
{
	return relifeCount <= 0;
}

void Soldier::resetRelife()
{
	relifeCount = 5;
}

void Soldier::countdownRelife()
{
	--relifeCount;
}

void Soldier::setSoldierPosition(Vec2 pos)
{
	base->setPosition(pos);
}

void Soldier::setTarget(Monster *tar)
{
	target = tar;
}

void Soldier::initSoldier()
{
	setSoldierPosition(Vec2(0, 0));
	setState(Soldier::SoldierState::stateBorn);
	setCurHP(getMaxHP());
	setTarget(nullptr);
	hpBarBg->setVisible(true);
}

void Soldier::walkTo(Vec2 pos)
{
	auto delta = pos - base->getPosition();
	//walkBy(delta);
	if (delta.x < 0)
		base->setFlippedX(true);
	else
		base->setFlippedX(false);
	std::string animationName = String::createWithFormat("soldier_walkingRightLv%d", level)->getCString();
	auto animationWalk = AnimationCache::getInstance()->getAnimation(animationName);
	animationWalk->setLoops(3);
	animateWalk = Animate::create(animationWalk);
	auto move = MoveTo::create(1.0f, pos);
	base->runAction(Spawn::create(move, animateWalk, nullptr));
}

void Soldier::walkBy(Vec2 delta)
{
	if (delta.x < 0)
		base->setFlippedX(true);
	else
		base->setFlippedX(false);
	std::string animationName = String::createWithFormat("soldier_walkingRightLv%d", level)->getCString();
	auto animationWalk = AnimationCache::getInstance()->getAnimation(animationName);
	animationWalk->setLoops(3);
	animateWalk = Animate::create(animationWalk);
	auto move = MoveBy::create(1.0f, delta);
	base->runAction(Spawn::create(move, animateWalk, nullptr));
}

void Soldier::attack(float dt)
{
	if (target->isDead()) {
		
		unschedule(schedule_selector(Soldier::attack));
		setState(stateReady);
		if (!searchMonster()) {
			walkTo(rallyPos);
		}
		target = nullptr;
		return;
	}
	std::string animationName = String::createWithFormat("soldier_attackLv%d", level)->getCString();
	auto animationAttack = AnimationCache::getInstance()->getAnimation(animationName);
	animateAttack = Animate::create(animationAttack);
	auto hitOn = [=]() {
		//target->getHurt();
		if(target)
			target->declineHp(power);
	};
	base->runAction(Sequence::create(animateAttack, CallFunc::create(hitOn), nullptr));
}

void Soldier::death()
{
	base->stopAllActions();
	state = stateDeath;
	std::string animationName = String::createWithFormat("soldier_deathLv%d", level)->getCString();
	auto animationDeath = AnimationCache::getInstance()->getAnimation(animationName);
	animateDeath = Animate::create(animationDeath);
	hpBarBg->setVisible(false);
	base->runAction(animateDeath);
}
