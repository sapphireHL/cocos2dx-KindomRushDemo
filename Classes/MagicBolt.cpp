#include "MagicBolt.h"
#include "MagicCurve.h"

MagicBolt * MagicBolt::creat()
{
	auto bolt = new MagicBolt;
	if (bolt && bolt->init()) {
		bolt->autorelease();
		return bolt;
	}
	delete bolt;
	bolt = nullptr;
	return nullptr;
}

bool MagicBolt::init()
{
	if (!Sprite::init())
		return false;

	bulletSprite = Sprite::createWithSpriteFrameName("voodoo_proy_0001.png");
	this->addChild(bulletSprite);
	particle = MotionStreak::create(1.0f, 1, 15, Color3B::WHITE, "voodoo_proy_particle.png");
	this->addChild(particle);

	power = 0;

	return true;
}

void MagicBolt::shoot()
{
	auto streak = [=]() {
		particle->runAction(move->clone());
	};

	auto animation = AnimationCache::getInstance()->getAnimation("voodoo_run");
	action = Spawn::create(Animate::create(animation), move, nullptr);

	auto appear = [=]() {
		this->setVisible(true);
	};
	auto removeStone = [=]() {
		this->removeFromParent();
	};
	auto hitOn = [=]() {
		target->declineHp(power);
	};
	auto explosion = [=]() {
		auto exploAnimation = AnimationCache::getInstance()->getAnimation("voodoo_explosion");
		bulletSprite->runAction(Animate::create(exploAnimation));
	};
	bulletSprite->runAction(Sequence::create(DelayTime::create(0.5f),
		CallFunc::create(appear),
		CallFunc::create(streak), action, CallFunc::create(hitOn),
		CallFunc::create(explosion), DelayTime::create(0.5f),
		CallFunc::create(removeStone), nullptr));
	
	
}

void MagicBolt::setTargetPos(Vec2 pos)
{
	targetPos = pos;
}

void MagicBolt::setMove(ActionInterval * act)
{
	move = act;
}
