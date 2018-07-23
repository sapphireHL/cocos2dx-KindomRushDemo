#include "Stone.h"
#include "GameController.h"

Stone * Stone::creat()
{
	auto stone = new Stone;
	if (stone && stone->init()) {
		stone->autorelease();
		return stone;
	}
	delete stone;
	stone = nullptr;
	return nullptr;
}

bool Stone::init()
{
	if (!Sprite::init())
		return false;

	bulletSprite = Sprite::createWithSpriteFrameName("artillery_thrower_proy.png");
	this->addChild(bulletSprite);

	power = 0;
	range = 0;

	return true;
}

void Stone::shoot()
{
	auto appear = [=]() {
		this->setVisible(true);
	};
	auto removeStone = [=]() {
		this->removeFromParent();
	};
	auto hitOn = [=]() {
		auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
		for (auto mons : monsters) {
			auto pos = mons->getWorldPosition();
			//·¶Î§¹¥»÷
			if (targetPos.distance(pos) < range) {
				mons->declineHp(power);
			}
		}
	};
	auto explosion = [=]() {
		auto exploAnimation = AnimationCache::getInstance()->getAnimation("artillery_thrower_explosion");
		bulletSprite->runAction(Animate::create(exploAnimation));
	};
	bulletSprite->runAction(Sequence::create(DelayTime::create(0.3f), 
							CallFunc::create(appear),
							action, CallFunc::create(hitOn),
							CallFunc::create(explosion), DelayTime::create(0.5f),
							CallFunc::create(removeStone), nullptr));
}

void Stone::setRange(float ran)
{
	range = ran;
}

void Stone::setTargetPos(Vec2 pos)
{
	targetPos = pos;
}

