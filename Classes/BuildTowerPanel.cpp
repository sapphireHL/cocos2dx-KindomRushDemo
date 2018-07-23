#include "BuildTowerPanel.h"
#include "BuildTowerIcon.h"

BuildTowerPanel * BuildTowerPanel::createPanel()
{
	auto panel = new BuildTowerPanel;
	if (panel && panel->init()) {
		panel->autorelease();
		return panel;
	}
	CC_SAFE_DELETE(panel);
	return nullptr;
}

bool BuildTowerPanel::init()
{
	if (!Sprite::init())
		return false;

	ring = Sprite::createWithSpriteFrameName("gui_ring.png");
	ring->setScale(0);
	this->addChild(ring);
	auto archericon = BuildTowerIcon::createIcon(1);
	archericon->setPosition(40, 200);
	ring->addChild(archericon, 1);
	auto barrackicon = BuildTowerIcon::createIcon(2);
	barrackicon->setPosition(200, 200);
	ring->addChild(barrackicon, 1);
	auto mageicon = BuildTowerIcon::createIcon(3);
	mageicon->setPosition(40, 40);
	ring->addChild(mageicon, 1);
	auto artilleryicon = BuildTowerIcon::createIcon(4);
	artilleryicon->setPosition(200, 40);
	ring->addChild(artilleryicon, 1);

	//auto touchListener = EventListenerTouchOneByOne::create();
	//touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
	//	return true;
	//};
	//touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
	//	auto target = dynamic_cast<Sprite*>(event->getCurrentTarget());
	//	auto pos = target->convertTouchToNodeSpace(touch);
	//	Rect rect(0, 0, target->getContentSize().width, target->getContentSize().height);
	//	if (rect.containsPoint(pos)) {
	//		target->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0019.png"));
	//		touchListener->setSwallowTouches(true);
	//	}
	//};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, archericon);
	return true;
}

void BuildTowerPanel::inAnimation()
{
	auto scaleto = ScaleTo::create(0.1f, 0.7f);
	ring->runAction(scaleto);
}

void BuildTowerPanel::outAnimation()
{
	auto scaleto = ScaleTo::create(0.1f, 0.0f);
	ring->runAction(scaleto);
}
