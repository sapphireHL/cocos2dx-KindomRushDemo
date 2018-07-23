#include "Monster.h"
#include "GameController.h"


void Monster::setType(MonsterType n)
{
	type = n;
}

MonsterType Monster::getType()
{
	return type;
}

void Monster::update(float dt)
{
	//if (isDeath) return;
	if (state == stateDeath)
		return;
	if (curHP <= 0) {
		state = stateDeath;
	}
	hpBar->setPercentage(float(100 * curHP / maxHP));
	//路线上的当前点和下一个点
	Vec2 currpos, nextpos;
	if (state != stateAttacked) {
		currpos = getCurrentPosition();
		nextpos = getNextPosition();
		if (nextpos == ERROR_POSITION)
			return;
	}
	
	float deltax = nextpos.x - currpos.x, deltay = nextpos.y - currpos.y;
	auto moveby = MoveBy::create(dt, Vec2(deltax, deltay));
	
	switch (state)
	{
	case stateNone:
		break;
	case stateWalkRight:
		prevState = state;
		stateHandlerWalkRight(deltax, deltay);
		this->runAction(moveby);
		break;
	case stateWalkLeft:
		prevState = state;
		stateHandlerWalkLeft(deltax, deltay);
		this->runAction(moveby);
		break;
	case stateWalkUp:
		prevState = state;
		stateHandlerWalkUp(deltax, deltay);
		this->runAction(moveby);
		break;
	case stateWalkDown:
		prevState = state;
		stateHandlerWalkDown(deltax, deltay);
		this->runAction(moveby);
		break;
	case stateAttack:
		break;
	case stateAttacked:
		//unschedule(schedule_selector(Monster::update));
		schedule(schedule_selector(Monster::attack), 1.5f);
		break;
	case stateDeath:
		death();
		unscheduleAllSelectors();
		return;
	default:
		break;
	}
	//auto real = baseSprite->convertToWorldSpace(baseSprite->getPosition());
	//auto rea = this->getPosition();
	//cocos2d::log("monster coor:%f, %f\n", rea.x, rea.y);

	//auto delta = nextpos - currpos;
	//auto nextpos = baseSprite->convertToNodeSpace(route[curpos]);
	
	//baseSprite->setPosition(route[curpos]);
}

void Monster::death()
{
	auto animationDeath = AnimationCache::getInstance()->getAnimation(name + "_death");
	animateDeath = Animate::create(animationDeath);
	baseSprite->stopAllActions();
	hpBarBg->setVisible(false);
	blood->setVisible(false);
	auto remove = [=]() {
		baseSprite->removeFromParent();
		hpBarBg->removeFromParent();
		_isReleased = true;
	};
	baseSprite->runAction(Sequence::create(animateDeath, DelayTime::create(0.5f), CallFunc::create(remove), nullptr));
}

void Monster::attack(float dt)
{
	if (target->isDead()) {
		target = nullptr;
		unschedule(schedule_selector(Monster::attack));
		state = prevState;
		switch (state)
		{
		case stateWalkRight:
			walkRight();
			break;
		case stateWalkLeft:
			walkLeft();
			break;
		case stateWalkUp:
			walkUp();
			break;
		case stateWalkDown:
			walkDown();
			break;
		default:
			break;
		}
		return;
	}
	baseSprite->stopAllActions();
	auto delta = target->getWorldPosition() - getWorldPosition();
	if (delta.x > 0)
		baseSprite->setFlippedX(false);
	else
		baseSprite->setFlippedX(true);
	//baseSprite->setFlippedX(true);
	target->setState(Soldier::SoldierState::stateAttacked);
	auto animation = AnimationCache::getInstance()->getAnimation(name + "_attack");
	auto animate = Animate::create(animation);
	auto hitOn = [=]() {
		//target->getHurt();
		target->declineHp(power);
	};
	auto hit = CallFunc::create(hitOn);
	baseSprite->runAction(Sequence::create(animate, CallFunc::create([=]() 
	{
		target->declineHp(power);
	}), nullptr));
	//baseSprite->runAction(animate);
}

Vec2 Monster::getSize()
{
	return baseSprite->getContentSize();
}

void Monster::getHurt()
{
	auto hurt = [=]() {
		blood->setVisible(true);
	};
	auto disappear = [=]() {
		blood->setVisible(false);
	};
	baseSprite->runAction(Sequence::create(CallFunc::create(hurt), DelayTime::create(0.1f), CallFunc::create(disappear), nullptr));
}

void Monster::walkRight()
{
	baseSprite->stopAllActions();
	baseSprite->setFlippedX(true);
	auto animation = AnimationCache::getInstance()->getAnimation(name + "_walkingRightLeft");
	auto animate = Animate::create(animation);
	baseSprite->runAction(RepeatForever::create(animate));
}

void Monster::walkLeft()
{
	baseSprite->stopAllActions();
	baseSprite->setFlippedX(true);
	auto animation = AnimationCache::getInstance()->getAnimation(name + "_walkingRightLeft");
	auto animate = Animate::create(animation);
	baseSprite->runAction(RepeatForever::create(animate));
}

void Monster::walkUp()
{
	baseSprite->stopAllActions();
	//baseSprite->setFlippedX(true);
	auto animation = AnimationCache::getInstance()->getAnimation(name + "_walkingUp");
	auto animate = Animate::create(animation);
	baseSprite->runAction(RepeatForever::create(animate));
}

void Monster::walkDown()
{
	baseSprite->stopAllActions();
	//baseSprite->setFlippedX(true);
	auto animation = AnimationCache::getInstance()->getAnimation(name + "_walkingDown");
	auto animate = Animate::create(animation);
	baseSprite->runAction(RepeatForever::create(animate));
}

void Monster::stateHandlerWalkRight(int deltax, int deltay)
{
	if (abs(deltay) > abs(deltax)) {
		if (deltay > 0) {
			state = stateWalkUp;
			walkUp();
		}
		else {
			state = stateWalkDown;
			walkDown();
		}
	}
	//向左走
	else if (deltax < 0) {
		state = stateWalkLeft;
		walkLeft();
	}
}

void Monster::stateHandlerWalkUp(int deltax, int deltay)
{
	if (abs(deltax) > abs(deltay)) {
		if (deltax > 0) {
			state = stateWalkRight;
			walkRight();
		}
		else {
			state = stateWalkLeft;
			walkLeft();
		}
	}
	else if (deltay < 0) {
		state = stateWalkDown;
		walkDown();
	}
}

void Monster::stateHandlerWalkLeft(int deltax, int deltay)
{
	if (abs(deltay) > abs(deltax)) {
		if (deltay > 0) {
			state = stateWalkUp;
			walkUp();
		}
		else {
			state = stateWalkDown;
			walkDown();
		}
	}
	//向右走
	else if (deltax > 0) {
		state = stateWalkRight;
		walkRight();
	}
}

void Monster::stateHandlerWalkDown(int deltax, int deltay)
{
	if (abs(deltax) > abs(deltay)) {
		if (deltax > 0) {
			state = stateWalkRight;
			walkRight();
		}
		else {
			state = stateWalkLeft;
			walkLeft();
		}
	}
	else if (deltay > 0) {
		state = stateWalkUp;
		walkUp();
	}
}

bool Monster::isReleased()
{
	return _isReleased;
}

bool Monster::isDead()
{
	return state == stateDeath;
}

bool Monster::isAttacked()
{
	return state == stateAttacked;
}



void Monster::declineHp(int power)
{
	setCurHP(getCurHP() - power);
}

void Monster::setHpBar()
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
	hpBarBg->setPositionY(20);
	this->addChild(hpBarBg);
	
}

void Monster::setTarget(Soldier *sol)
{
	target = sol;
}

Soldier * Monster::getTarget()
{
	return target;
}

Vec2 Monster::getNextPosition()
{
	if (++curpos >= (int)route.size()) {
		return ERROR_POSITION;
	}
	return route[curpos];
}

Vec2 Monster::getCurrentPosition()
{
	if (curpos >= (int)route.size())
		return ERROR_POSITION;
	return route[curpos];
}

Vec2 Monster::getWorldPosition()
{
	auto bg = GameController::getInstance()->getBg();
	auto deltax = bg->getContentSize().width / 2 - bg->getPosition().x;
	auto deltay = bg->getContentSize().height / 2 - bg->getPosition().y;
	auto pos = this->getPosition() - Vec2(deltax, deltay);
	return pos;
}

//后面step步的位置
Vec2 Monster::getWorldPositionForecast(int step)
{
	if (curpos + step >= route.size()) {
		return ERROR_POSITION;
	}
	auto pos = route[curpos + step];
	auto bg = GameController::getInstance()->getBg();
	auto deltax = bg->getContentSize().width / 2 - bg->getPosition().x;
	auto deltay = bg->getContentSize().height / 2 - bg->getPosition().y;
	return pos - Vec2(deltax, deltay);
}

