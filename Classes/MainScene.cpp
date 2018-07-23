#include "MainScene.h"
#include "GameScene.h"

Scene * MainScene::createScene()
{
	return MainScene::create();
}

bool MainScene::init()
{
	if (!Scene::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto center = Size(visibleSize.width / 2, visibleSize.height / 2);

	//大地图背景
	auto bg = Sprite::createWithSpriteFrameName("MapBackground.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);

	//关卡旗子
	Vec2 pos_level1(202, bg->getContentSize().height - 220);
	auto flag0 = Sprite::createWithSpriteFrameName("mapFlag_0001.png");
	auto flag = Sprite::createWithSpriteFrameName("mapFlag_0002.png");
	flag0->setPosition(pos_level1);
	flag->setPosition(pos_level1);
	bg->addChild(flag0);
	bg->addChild(flag);

	//关卡选择
	auto selectLevel = EventListenerTouchOneByOne::create();
	selectLevel->onTouchBegan = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto pos = target->convertTouchToNodeSpace(touch);
		auto size = target->getContentSize();
		auto rect = Rect(0, 0, size.width, size.height);

		if (rect.containsPoint(pos)) {
			flag->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mapFlag_0003.png"));
		}
		return true;
	};
	selectLevel->onTouchEnded = [=](Touch* touch, Event* event) {
		flag->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mapFlag_0002.png"));
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto pos = target->convertTouchToNodeSpace(touch);
		auto size = target->getContentSize();
		auto rect = Rect(0, 0, size.width, size.height);

		if (rect.containsPoint(pos)) {
			auto newscene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newscene));
		}
	};
	selectLevel->setSwallowTouches(false);

	//拖动地图
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesMoved = [=](std::vector<Touch*> touches, Event* event) {
		if (touches.size() == 1) {
			auto touch = touches[0];
			auto diff = touch->getDelta();
			auto curpos = bg->getPosition();
			auto newpos = curpos + diff;
			auto bgsize = bg->getContentSize();
			//x,y约束，控制边界
			newpos.x = std::min(newpos.x, center.width + (bgsize.width - visibleSize.width) / 2);
			newpos.x = std::max(newpos.x, center.width - (bgsize.width - visibleSize.width) / 2);
			newpos.y = std::min(newpos.y, center.height + (bgsize.height - visibleSize.height) / 2);
			newpos.y = std::max(newpos.y, center.height - (bgsize.height - visibleSize.height) / 2);
			bg->setPosition(newpos);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, bg);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(selectLevel, flag);

	return true;
}
