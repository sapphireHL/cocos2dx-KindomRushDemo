#include "SkillLightening.h"
#include "GameController.h"

SkillLightening * SkillLightening::createSkillItem()
{
	auto skill = new SkillLightening;
	if (skill&&skill->init()) {
		skill->autorelease();
		return skill;
	}
	CC_SAFE_DELETE(skill);
	return nullptr;
}

bool SkillLightening::init()
{
	if (!SkillItem::init())
		return false;

	item = Sprite::createWithSpriteFrameName("portraits_power_lightening.png");
	itemSelected = Sprite::createWithSpriteFrameName("portraits_power_selected.png");
	itemSelected->setVisible(false);
	itemCool = Sprite::createWithSpriteFrameName("power_loading.png");
	this->addChild(item);
	this->addChild(itemSelected);
	coolTime = 30.0f;
	isCool = false;

	coolTimer = ProgressTimer::create(itemCool);
	coolTimer->setPercentage(100);
	coolTimer->setType(ProgressTimerType::BAR);
	coolTimer->setVisible(false);
	coolTimer->setMidpoint(Vec2(0, 1));
	coolTimer->setBarChangeRate(Vec2(0, 1));
	this->addChild(coolTimer);
	
	auto touchSkill = EventListenerTouchOneByOne::create();
	touchSkill->onTouchBegan = [=](Touch* touch, Event* event) {
		return true;
	};
	touchSkill->onTouchEnded = [=](Touch* touch, Event* event) {
		if (isCool) 
			return;
		auto target = dynamic_cast<Sprite*>(event->getCurrentTarget());
		auto loc = touch->getLocation();
		auto pos = target->convertTouchToNodeSpace(touch);
		auto rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
		if (selected()) {
			if (!rect.containsPoint(pos)) {
				applySkill(loc);
			}
			unselect();
		}
		else {
			if (rect.containsPoint(pos)) {
				select();
			}
			else {
				unselect();
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchSkill, item);

	return true;
}

void SkillLightening::applySkill(Vec2 pos)
{
	auto lightening = Sprite::createWithSpriteFrameName("ray_0001.png");
	lightening->setAnchorPoint(Vec2(0.5,0));
	this->addChild(lightening);
	lightening->setPosition(pos-this->getPosition());

	auto hit = [=]() {
		auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
		for (auto mons : monsters) {
			auto monsPos = mons->getWorldPosition();
			if (pos.distance(monsPos) < 80) {
				mons->declineHp(30);
			}
		}
	};
	auto explo = [=]() {
		auto explosionLeft = Sprite::createWithSpriteFrameName("ray_explosion_half_0001.png");
		auto exploAnimation = AnimationCache::getInstance()->getAnimation("lightning_storm_explosion");
		explosionLeft->setPosition(pos - this->getPosition());
		this->addChild(explosionLeft);
		explosionLeft->runAction(Sequence::create(Animate::create(exploAnimation), CallFunc::create([=]()
		{
			explosionLeft->removeFromParent();
		}), nullptr));

		auto explosionRight = Sprite::createWithSpriteFrameName("ray_explosion_half_0001.png");
		explosionRight->setPosition(pos - this->getPosition());
		explosionRight->setFlippedX(true);
		this->addChild(explosionRight);
		explosionRight->runAction(Sequence::create(Animate::create(exploAnimation), CallFunc::create([=]()
		{
			explosionRight->removeFromParent();
		}), nullptr));
	};
	auto changeP2 = [=]() {
		lightening->setOpacity(255);
		lightening->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_0002.png"));
		hit();
		explo();
	};
	auto changeP3 = [=]() {
		lightening->setOpacity(255);
		lightening->setFlippedX(true);
		hit();
		explo();
	};
	auto remove = [=]() {
		lightening->removeFromParent();
	};
	hit();
	explo();
	lightening->runAction(Sequence::create(FadeOut::create(0.4f), 
		CallFunc::create(changeP2), FadeOut::create(0.4f), 
		CallFunc::create(changeP3), FadeOut::create(0.4f),
		CallFunc::create(remove), nullptr));

	coolTimer->setVisible(true);
	coolTimer->setPercentage(100);
	schedule(schedule_selector(SkillLightening::cool), 1.0*coolTime/1000);
}

void SkillLightening::cool(float dt)
{
	isCool = true;
	float curPer = coolTimer->getPercentage();
	if (abs(curPer - 0) < 1e-5) {
		isCool = false;
		coolTimer->setVisible(false);
		unschedule(schedule_selector(SkillLightening::cool));
	}
	coolTimer->setPercentage(curPer - 0.1);
}
