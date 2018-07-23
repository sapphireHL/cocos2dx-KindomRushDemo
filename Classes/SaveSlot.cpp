#include "SaveSlot.h"
#include "MainScene.h"

SaveSlot::SaveSlot()
{
}

SaveSlot::~SaveSlot()
{
}

bool SaveSlot::initWithNum(int num)
{
	if (!Sprite::init())
		return false;

	setNum(num);
	slot = Sprite::createWithSpriteFrameName("mainmenu_saveslot_0001.png");
	this->addChild(slot);

	init_button_slot_delete();
	init_delete_menu();

	if (UserDefault::getInstance()->getIntegerForKey(String::createWithFormat("Slot%d", num)->getCString()) == 0) {
		createNewGame();
		button_slot_delete->setVisible(false);
	}
	else {
		showRecord();
	}

	auto slot_listener = EventListenerTouchOneByOne::create();
	slot_listener->onTouchBegan = [&](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		auto pos = target->convertTouchToNodeSpace(touch);
		auto size = target->getContentSize();
		auto rect = Rect(0, 0, size.width, size.height);

		if (rect.containsPoint(pos) && !slot->getChildByTag(TAG_DELETE)->isVisible()) {
			target->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainmenu_saveslot_0002.png"));
			return true;
		}
		return false;
	};

	slot_listener->onTouchEnded = [&](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mainmenu_saveslot_0001.png"));
		auto pos = target->convertTouchToNodeSpace(touch);
		auto size = target->getContentSize();
		auto rect = Rect(0, 0, size.width, size.height);

		if (rect.containsPoint(pos) && !slot->getChildByTag(TAG_DELETE)->isVisible()) {
			UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d", getNum())->getCString(), 1);
			auto newScene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene));
		}
	};
	slot_listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(slot_listener, slot);
	return true;
}

SaveSlot * SaveSlot::createSlot(int num)
{
	auto slotMenu = new SaveSlot();
	if (slotMenu && slotMenu->initWithNum(num)) {
		slotMenu->autorelease();
		return slotMenu;
	}
	CC_SAFE_DELETE(slotMenu);
	return NULL;
}

void SaveSlot::init_button_slot_delete()
{
	button_slot_delete = MenuItemSprite::create(Sprite::createWithSpriteFrameName("mainmenu_saveslot_delete_0001.png"),
		Sprite::createWithSpriteFrameName("mainmenu_saveslot_delete_0002.png"),
		CC_CALLBACK_1(SaveSlot::callback_slot_delete, this));
	button_slot_delete->setPosition(slot->getContentSize().width / 2, slot->getContentSize().height / 2);
	auto menu = Menu::create(button_slot_delete, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void SaveSlot::init_delete_menu()
{
	auto label_hint = Label::createWithTTF("DELETE SLOT?", "fonts/Comic Book Italic.ttf", 32.0f);
	label_hint->setColor(Color3B(219, 205, 139));
	label_hint->setPosition(slot->getContentSize().width / 2, slot->getContentSize().height / 1.5);

	auto comfirm_delete = MenuItemSprite::create(Sprite::createWithSpriteFrameName("mainmenu_saveslot_confirmdelete_0001.png"),
		Sprite::createWithSpriteFrameName("mainmenu_saveslot_confirmdelete_0001.png"),
		CC_CALLBACK_1(SaveSlot::callback_confirm_delete, this));
	comfirm_delete->setPosition(slot->getContentSize().width / 4, slot->getContentSize().height / 3);

	auto cancel_delete = MenuItemSprite::create(Sprite::createWithSpriteFrameName("mainmenu_saveslot_confirmdelete_0002.png"),
		Sprite::createWithSpriteFrameName("mainmenu_saveslot_confirmdelete_0002.png"),
		CC_CALLBACK_1(SaveSlot::callback_cancel_delete, this));
	cancel_delete->setPosition(slot->getContentSize().width / 4 * 3, slot->getContentSize().height / 3);

	auto delete_menu = Menu::create(comfirm_delete, cancel_delete, NULL);
	delete_menu->setPosition(Vec2::ZERO);

	auto delete_ui = Sprite::create();
	delete_ui->addChild(label_hint);
	delete_ui->addChild(delete_menu);
	delete_ui->setTag(TAG_DELETE);
	delete_ui->setVisible(false);
	slot->addChild(delete_ui);
}

void SaveSlot::showRecord()
{
	auto record = Sprite::create();
	auto label_slot = Label::createWithTTF(String::createWithFormat("SLOT%d", num)->getCString(), "fonts/Comic Book Italic.ttf", 32.0f);
	label_slot->setColor(Color3B(219, 205, 139));
	label_slot->setPosition(slot->getContentSize().width / 2, slot->getContentSize().height / 1.5);
	record->addChild(label_slot);
	auto star = Sprite::createWithSpriteFrameName("mainmenu_saveslot_icons_0001.png");
	star->setPosition(slot->getContentSize().width / 4, slot->getContentSize().height / 3);
	record->addChild(star);
	int starCount = UserDefault::getInstance()->getIntegerForKey(String::createWithFormat("Slot%d_star")->getCString());
	auto count = Label::createWithTTF(String::createWithFormat("%d/65", starCount)->getCString(), "fonts/Comic Book Italic.ttf", 32.0f);
	count->setColor(Color3B(219, 205, 139));
	count->setPosition(slot->getContentSize().width / 4 * 2.5, slot->getContentSize().height / 3);
	record->addChild(count);
	record->setTag(TAG_NEWGAME);
	slot->addChild(record);
}

void SaveSlot::deleteRecord(int num)
{
	UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d", getNum())->getCString(), 0);

	for (int i = 0; i<13; i++)
		UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_Level_%d", getNum(), i)->getCString(), 0);

	for (int i = 0; i<13; i++)
		UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_Level_%d_star", getNum(), i)->getCString(), 0);

	UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_DownCount", getNum())->getCString(), 0);
	UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_NewDown", getNum())->getCString(), 0);
	UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_StarLeft", getNum())->getCString(), 0);
	UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_Star", getNum())->getCString(), 0);
	UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_Gem", getNum())->getCString(), 0);

	for (int i = 0; i<6; i++) {
		UserDefault::getInstance()->setIntegerForKey(String::createWithFormat("Slot%d_Shop_%d", getNum(), i)->getCString(), 0);
	}
}

void SaveSlot::createNewGame()
{
	auto label_new = Label::createWithTTF("new game", "fonts/Comic Book Italic.ttf", 42);
	label_new->setColor(Color3B(219, 205, 139));
	label_new->setPosition(slot->getContentSize().width / 2, slot->getContentSize().height / 2);
	label_new->setTag(TAG_NEWGAME);
	slot->addChild(label_new);
}

void SaveSlot::callback_slot_delete(Ref * pSender)
{
	slot->getChildByTag(TAG_NEWGAME)->setVisible(false);
	button_slot_delete->setVisible(false);
	slot->getChildByTag(TAG_DELETE)->setVisible(true);
}

void SaveSlot::callback_confirm_delete(Ref * pSender)
{
	slot->getChildByTag(TAG_DELETE)->setVisible(false);
	slot->removeChildByTag(TAG_NEWGAME);
	createNewGame();
	deleteRecord(getNum());
}

void SaveSlot::callback_cancel_delete(Ref * pSender)
{
	slot->getChildByTag(TAG_NEWGAME)->setVisible(true);
	button_slot_delete->setVisible(true);
	slot->getChildByTag(TAG_DELETE)->setVisible(false);
}
