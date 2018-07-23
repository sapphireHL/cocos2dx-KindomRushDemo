#include "LoadingScene.h"
#include "WelcomeScene.h"

using namespace cocos2d;

cocos2d::Scene * LoadingScene::createScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init()
{
	if (!Scene::init())
		return false;

	loaded = 0;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto bg = Sprite::create("Default@2x.png");
	bg->setPosition(center);
	this->addChild(bg);

	scheduleUpdate();

	return true;
}

void LoadingScene::loadSource()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainmenu_spritesheet_32_1-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainmenu_spritesheet_32_2-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainmenu_spritesheet_32_localized_de-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map_spritesheet_16_na-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map_spritesheet_32_2-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map_spritesheet_32_3-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map_spritesheet_32-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite_level1_2-hd.plist");

	

	loaded++;
}

void LoadingScene::update(float delta)
{
	if (loaded == 0)
		loadSource();
	else if (loaded == 1) {
		auto scene = WelcomeScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}


