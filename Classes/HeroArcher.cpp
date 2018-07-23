#include "HeroArcher.h"
#include "Monster.h"
#include "GameController.h"
#include "Arrow.h"

HeroArcher * HeroArcher::createHero()
{
	auto hero = new HeroArcher;
	if (hero && hero->init()) {
		hero->autorelease();
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return nullptr;
}

bool HeroArcher::init()
{
	if (!Sprite::init())
		return false;

	isDeath = false;
	isReleased = false;

	base = Sprite::createWithSpriteFrameName("archer_hero_0001.png");
	this->addChild(base);
	setHpBar();

	state = stateIdleBow;
	birthPlace = Vec2(100, 500);
	//birthPlace = Vec2(900, 750);
	base->setPosition(birthPlace);

	setMaxHP(100);
	setCurHP(100);
	setPower(10);

	target = nullptr;
	relifeCount = 20;

	//初始动作
	idle();

	heroSelected = Sprite::createWithSpriteFrameName("selected_soldier_small.png");
	heroSelected->setVisible(false);
	heroSelected->setPosition(base->getContentSize().width / 2, 20);
	base->addChild(heroSelected);

	//设置触摸
	touchCnt = 0;
	confirm= Sprite::createWithSpriteFrameName("confirm_feedback_layer2_0001.png");
	confirm->setOpacity(0);
	this->addChild(confirm);
	auto touchHero = EventListenerTouchOneByOne::create();
	touchHero->onTouchBegan = [=](Touch* touch, Event* event) {
		return true;
	};
	touchHero->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = dynamic_cast<Sprite*>(event->getCurrentTarget());
		auto touchPosLInWindow = touch->getLocation();
		auto basePosInWindow = convertToWorldSpace(target->getPosition());
		auto delta = touchPosLInWindow - basePosInWindow;
		auto pos = target->convertTouchToNodeSpace(touch);
		auto rect = Rect(80, 0, 60,80);
		if (touchCnt == 1) {
			confirm->setPosition(base->getPosition() + delta - Vec2(0, 20));
			confirm->runAction(Sequence::create(FadeIn::create(0.1f), DelayTime::create(0.2f), FadeOut::create(0.1f), nullptr));
			walkBy(delta);
			touchCnt = 0;
			heroSelected->setVisible(false);
		}
		else {
			if (rect.containsPoint(pos)) {
				heroSelected->setVisible(true);
				++touchCnt;
			}
			else {
				heroSelected->setVisible(false);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchHero, base);
	schedule(schedule_selector(HeroArcher::update), 0.1f);

	return true;
}

void HeroArcher::update(float dt)
{
	if (state == stateWalk)
		return;
	hpBar->setPercentage(float(100 * curHP / maxHP));
	//有目标
	//if (target) {
	//	if (isTooNear()) {
	//		state = stateAttackSword;
	//		base->stopAllActions();
	//		auto monsterPos = target->getWorldPosition();
	//		walkBy(0.75*(monsterPos - convertToWorldSpace(base->getPosition())));
	//		target->setState(MonsterState::stateAttacked);
	//		schedule(schedule_selector(HeroArcher::attackSword), 1.0f);
	//	}
	//	else {
	//		state = stateAttackBow;
	//		schedule(schedule_selector(HeroArcher::shootBow), 1.0f);
	//	}
	//}
	//若没有目标就寻找
	//if (!target) {
		if (searchMonster()) {
			//太远射击
			if (state == stateAttackBow) {
				schedule(schedule_selector(HeroArcher::shootBow), 1.0f);
			}
			//太近近战
			else if (state == stateAttackSword) {
				base->stopAllActions();
				auto monsterPos = target->getWorldPosition();
				walkBy(0.75*(monsterPos - convertToWorldSpace(base->getPosition())));
				target->setState(MonsterState::stateAttacked);
				schedule(schedule_selector(HeroArcher::attackSword), 1.0f);
			}
		}
		else {
			//unschedule(schedule_selector(HeroArcher::shootBow));
			//unschedule(schedule_selector(HeroArcher::attackSword));
		}
	//}
}

void HeroArcher::walkBy(Vec2 pos)
{
	state = stateWalk;
	base->stopAllActions();
	if (pos.x > 0)
		base->setFlippedX(false);
	else
		base->setFlippedX(true);
	auto walkAnimation = AnimationCache::getInstance()->getAnimation("archer_hero_walk");
	base->runAction(RepeatForever::create(Animate::create(walkAnimation)));
	float dis = pos.distance(Vec2(0, 0));
	float time = dis / 100;
	auto move = MoveBy::create(time, pos);
	auto stopWalk = [=]() {
		base->stopAllActions();
		idle();
	};
	base->runAction(Sequence::create(move, CallFunc::create(stopWalk), nullptr));
}

void HeroArcher::idle()
{
	auto idleAnimation = AnimationCache::getInstance()->getAnimation("archer_hero_idleBow");
	base->runAction(RepeatForever::create(Animate::create(idleAnimation)));
	state = stateIdleBow;
}

bool HeroArcher::searchMonster()
{
	target = nullptr;
	auto heroPos = convertToWorldSpace(base->getPosition());
	auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
	for (int i = 0; i < monsters.size(); ++i) {
		if (monsters[i]->isDead())
			continue;
		auto monsterPos = monsters[i]->getWorldPosition();
		float dis = heroPos.distance(monsterPos);
		if (dis < 200) {
			target = monsters[i];
			state = stateAttackBow;
			//if (dis < 20) {
			//	state = stateAttackSword;
			//	if (monsters[i]->getTarget() == nullptr)
			//		monsters[i]->setTarget(this);
			//}
			//else
			//	state = stateAttackBow;
			return true;
		}
	}
	state = stateIdleBow;
	return false;
}

void HeroArcher::shootBow(float dt)
{
	if (state != stateAttackBow)
		return;
	if (target->isDead()) {
		unschedule(schedule_selector(HeroArcher::shootBow));
		idle();
		target = nullptr;
		return;
	}
	base->stopAllActions();
	auto shootAnimation = AnimationCache::getInstance()->getAnimation("archer_hero_shootStart");
	auto shootStart = Animate::create(shootAnimation);
	shootAnimation = AnimationCache::getInstance()->getAnimation("archer_hero_shootEnd");
	auto shootEnd = Animate::create(shootAnimation);
	Vec2 shootPos;//射击位置
	if (target->isAttacked()) {
		shootPos = target->getWorldPosition();
	}
	else
	{
		shootPos = target->getWorldPositionForecast(2);
		if(shootPos == ERROR_POSITION)
			shootPos = target->getWorldPosition();
	}
	auto delta = shootPos - convertToWorldSpace(base->getPosition());
	
	//箭
	auto arrow = Arrow::creat();
	arrow->setPosition(base->getPosition());
	this->addChild(arrow);

	if (delta.x > 0) {
		base->setFlippedX(false);
	}
	else {
		base->setFlippedX(true);
	}
	arrow->calcRoute(delta);
	arrow->setTarget(target);
	arrow->setPower(power);
	base->runAction(Sequence::create(shootStart, shootEnd, nullptr));
	arrow->shoot();
}

void HeroArcher::attackSword(float dt)
{
	if (state != stateAttackSword)
		return;
	if (target->isDead()) {
		unschedule(schedule_selector(HeroArcher::attackSword));
		idle();
		target = nullptr;
		return;
	}
	base->stopAllActions();
	auto swordAnimation = AnimationCache::getInstance()->getAnimation("archer_hero_attack");
	auto attack = Animate::create(swordAnimation);
	auto hitOn = [=]() {
		target->declineHp(power);
	};
	base->runAction(Sequence::create(animateAttack, CallFunc::create(hitOn), nullptr));
}

bool HeroArcher::isTooNear()
{
	if (target) {
		auto heroPos = convertToWorldSpace(base->getPosition());
		auto monsterPos = target->getWorldPosition();
		float dis = heroPos.distance(monsterPos);
		if (dis < 20)
			return true;
		return false;
	}
}
