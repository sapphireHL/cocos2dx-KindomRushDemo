#include "WelcomeScene.h"
#include "SaveSlot.h"

using namespace cocos2d;

cocos2d::Scene * WelcomeScene::createScene()
{
	return WelcomeScene::create();
}

bool WelcomeScene::init()
{
	if (!Scene::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto position = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	init_background();
	init_logo();
	init_startButton();
	init_save_menu();
	
	
	return true;
}

void WelcomeScene::init_background()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto position = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto bg = Sprite::createWithSpriteFrameName("mainmenu_bg.png");
	bg->setPosition(position);
	this->addChild(bg, -1);
}

void WelcomeScene::init_logo()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	position_logo = Vec2(visibleSize.width / 2, visibleSize.height / 1.5);

	auto logo = Sprite::createWithSpriteFrameName("logo.png");
	logo->setPosition(position_logo);
	logo->setScale(0.8);
	this->addChild(logo, 2);

	//缩放动画
	auto scaleto = ScaleTo::create(0.5f, 1.0f);
	//平滑
	logo->runAction(EaseBackOut::create(scaleto));

	//生成闪烁动画
	SpriteFrame* frame;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i <= 21; i++) {
		frame = SpriteFrameCache::getInstance()->spriteFrameByName(String::createWithFormat("logo_brillo_00%02d.png", i)->getCString());
		if (frame)
			frames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(frames, 0.03f);
	auto brillo = Animate::create(animation);

	//播放动画
	auto sprite = Sprite::createWithSpriteFrameName("logo_brillo_0001.png");
	sprite->setPosition(position_logo);
	auto delay = DelayTime::create(1.0f);
	auto sequence_brillo = Sequence::create(delay, brillo, delay, nullptr);
	sprite->runAction(RepeatForever::create(sequence_brillo));
	this->addChild(sprite, 2);

}

void WelcomeScene::init_startButton()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto position = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto btn_start = Sprite::createWithSpriteFrameName("menu_startchain_0001.png");
	auto btn_start_selected = Sprite::createWithSpriteFrameName("menu_startchain_0002.png");

	auto menu_start = MenuItemSprite::create(btn_start, btn_start_selected, CC_CALLBACK_1(WelcomeScene::callback_start, this));
	auto menu = Menu::create(menu_start, nullptr);
	menu->setPosition(position_logo);

	//滑动
	auto moveto = MoveTo::create(0.5f, Vec2(position_logo.x, position.y - 70));
	auto slide = EaseBackOut::create(moveto);
	auto logo_delay = DelayTime::create(0.5f);
	auto delay = DelayTime::create(1.0f);
	menu->runAction(Sequence::create(logo_delay, slide, delay, nullptr));
	this->addChild(menu, 1);
	
}

void WelcomeScene::init_save_menu()
{
	//加载背景图片
	menu_save = Sprite::createWithSpriteFrameName("mainmenu_saveslot_bg.png");
	//计算位置
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 position = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	//初始位置屏幕外
	menu_save->setPosition(Vec2(position.x, position.y - 1000));
	auto panel_size = menu_save->getContentSize();
	//生成3个存档
	auto slot1 = SaveSlot::createSlot(1);
	slot1->setPosition(panel_size.width / 2, panel_size.height / 2 + 160);;
	menu_save->addChild(slot1);
	auto slot2 = SaveSlot::createSlot(2);
	slot2->setPosition(panel_size.width / 2, panel_size.height / 2);
	menu_save->addChild(slot2);
	auto slot3 = SaveSlot::createSlot(3);
	slot3->setPosition(panel_size.width / 2, panel_size.height / 2 - 160);
	menu_save->addChild(slot3);

	//加载关闭按钮
	auto button_close = Sprite::createWithSpriteFrameName("mainmenu_saveslot_close_0001.png");
	auto button_close_selected = Sprite::createWithSpriteFrameName("mainmenu_saveslot_close_0002.png");
	auto label_close = Label::createWithTTF("close", "fonts/Comic Book Italic.ttf", 32.0f);
	label_close->setColor(Color3B(219, 205, 139));
	label_close->setPosition(button_close->getContentSize().width / 2, button_close->getContentSize().height / 2 + 10);
	button_close->addChild(label_close);
	auto saveslot_close = MenuItemSprite::create(button_close, button_close_selected, CC_CALLBACK_1(WelcomeScene::callback_close, this));
	auto menu_close = Menu::create(saveslot_close, nullptr);
	menu_close->setPosition(menu_save->getContentSize().width / 2, -20);
	menu_save->addChild(menu_close);

	this->addChild(menu_save, 3);
}

void WelcomeScene::save_menu_slide_in()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto moveto = MoveTo::create(0.5f, Vec2(visibleSize.width / 2, visibleSize.height / 2 + 20));
	auto slide_in = EaseBackOut::create(moveto);
	menu_save->runAction(slide_in);
}

void WelcomeScene::save_menu_slide_out()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto moveto = MoveTo::create(0.5f, Vec2(visibleSize.width / 2, visibleSize.height / 2 - 1000));
	auto slide_out = EaseBackOut::create(moveto);
	menu_save->runAction(slide_out);
}

void WelcomeScene::callback_start(Ref * pSender)
{
	save_menu_slide_in();
}

void WelcomeScene::callback_close(Ref * pSender)
{
	save_menu_slide_out();
}
