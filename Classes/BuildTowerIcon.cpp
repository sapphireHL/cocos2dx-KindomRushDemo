#include "BuildTowerIcon.h"


BuildTowerIcon * BuildTowerIcon::createIcon(int type)
{
	auto icon = new BuildTowerIcon;
	if (icon && icon->init(type)) {
		icon->autorelease();
		return icon;
	}
	CC_SAFE_DELETE(icon);
	return nullptr;
}

bool BuildTowerIcon::init(int type)
{
	if (!Sprite::init())
		return false;

	isTouched = false;
	isBuildReady = false;
	icon = Sprite::createWithSpriteFrameName("main_icons_over.png");
	

	this->setContentSize(icon->getContentSize());

	preview = Sprite::createWithSpriteFrameName("tower_preview_archer.png");
	preview->setVisible(false);
	preview->setOpacity(150);
	this->addChild(preview);

	
	switch (type)
	{
	case ICON_TYPE_ARCHER:
		tower = Sprite::createWithSpriteFrameName("main_icons_0100.png");
		tower->setTag(ICON_TYPE_ARCHER);
		break;
	case ICON_TYPE_BARRACKS:
		tower = Sprite::createWithSpriteFrameName("main_icons_0101.png");
		tower->setTag(ICON_TYPE_BARRACKS);
		break;
	case ICON_TYPE_MAGE:
		tower = Sprite::createWithSpriteFrameName("main_icons_0102.png");
		tower->setTag(ICON_TYPE_MAGE);
		break;
	case ICON_TYPE_ARTILLERY:
		tower = Sprite::createWithSpriteFrameName("main_icons_0103.png");
		tower->setTag(ICON_TYPE_ARTILLERY);
		break;
	case ICON_TYPE_UPGRADE:
		tower = Sprite::createWithSpriteFrameName("main_icons_0005.png");
		tower->setTag(ICON_TYPE_UPGRADE);
		break;
	case ICON_TYPE_SELL:
		tower = Sprite::createWithSpriteFrameName("ico_sell_0001.png");
		tower->setTag(ICON_TYPE_SELL);
		break;
	case ICON_TYPE_RERALLY:
		tower = Sprite::createWithSpriteFrameName("sub_icons_0001.png");
		tower->setTag(ICON_TYPE_RERALLY);
		break;
	default:
		break;
	}
	if (type >= 1 && type <= 5) {
		tower->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
		icon->addChild(tower);
		this->addChild(icon);
	}
	else {
		this->addChild(tower);
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {

		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = dynamic_cast<Sprite*>(event->getCurrentTarget());
		auto pos = target->convertTouchToNodeSpace(touch);
		auto rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
		auto copy = tower->getSpriteFrame()->clone();
		if (rect.containsPoint(pos)) {
			if (isTouched) {
				//´´½¨
				isBuildReady = true;
				//preview->setVisible(false);
				//isTouched = false;
				//touchListener->setSwallowTouches(false);
			}
			else {
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0019.png"));
				switch (tower->getTag())
				{
				case ICON_TYPE_ARCHER:
					preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_archer.png"));
					break;
				case ICON_TYPE_BARRACKS:
					preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_barracks.png"));
					break;
				case ICON_TYPE_MAGE:
					preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_mage.png"));
					break;
				case ICON_TYPE_ARTILLERY:
					preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_artillery.png"));
					break;
				case ICON_TYPE_SELL:
					tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ico_sell_0002.png"));
					break;
				default:
					break;
				}
				//preview->setVisible(true);
				isTouched = true;
			}
		}
		else {
			switch (tower->getTag())
			{
			case ICON_TYPE_ARCHER:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0100.png"));
				break;
			case ICON_TYPE_BARRACKS:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0101.png"));
				break;
			case ICON_TYPE_MAGE:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0102.png"));
				break;
			case ICON_TYPE_ARTILLERY:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0103.png"));
				break;
			case ICON_TYPE_UPGRADE:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("main_icons_0005.png"));
				break;
			case ICON_TYPE_SELL:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ico_sell_0001.png"));
				break;
			case ICON_TYPE_RERALLY:
				tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("sub_icons_0001.png"));
				break;
			default:
				break;
			}
			preview->setVisible(false);
			isTouched = false;
			isBuildReady = false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, tower);
	return true;
}

bool BuildTowerIcon::isSelected()
{
	return isTouched;
}

bool BuildTowerIcon::isDoubleSelected()
{
	return isBuildReady;
}
