#include "BuildTerrain.h"
#include "ArcherTower.h"
#include "ArcherTowerLv2.h"
#include "BarrackTower.h"
#include "ArtilleryTower.h"
#include "MageTower.h"
#include "GameController.h"
#include <typeinfo>

BuildTerrain * BuildTerrain::createTerrain(Vec2 pos)
{
	auto terrain = new BuildTerrain;
	if (terrain && terrain->init()) {
		terrain->rallyPos = pos;
		terrain->autorelease();
		return terrain;
	}
	CC_SAFE_DELETE(terrain);
	return nullptr;
}

bool BuildTerrain::init()
{
	if (!Sprite::init())
		return false;
	this->setAnchorPoint(Vec2(0.5, 0.5));

	isMenuShown = false;
	isBuilt = false;
	buildType = -1;
	selectedType = -1;
	towerType = -1;
	terrain = Sprite::createWithSpriteFrameName("build_terrain_0001.png");
	this->addChild(terrain, 1);

	buildPanel = Sprite::createWithSpriteFrameName("gui_ring.png");
	buildPanel->setScale(0);
	buildPanel->setZOrder(7);
	this->addChild(buildPanel);

	upgradePanel = Sprite::createWithSpriteFrameName("gui_ring.png");
	upgradePanel->setScale(0);
	upgradePanel->setZOrder(7);
	this->addChild(upgradePanel);

	int icon_pos[4][2] = { { 90,250 },{ 250,250 },{ 90,90 },{ 250,90 } };
	for (int i = 0; i < 4; i++) {
		icons[i] = BuildTowerIcon::createIcon(i + 1);
		icons[i]->setPosition(icon_pos[i][0], icon_pos[i][1]);
		buildPanel->addChild(icons[i], 1);
	}

	upgradeIcon = BuildTowerIcon::createIcon(5);
	upgradeIcon->setPosition(170,270);
	upgradePanel->addChild(upgradeIcon);
	sellIcon = BuildTowerIcon::createIcon(6);
	sellIcon->setPosition(170, 70);
	upgradePanel->addChild(sellIcon);
	reRallyIcon = BuildTowerIcon::createIcon(7);
	reRallyIcon->setVisible(false);
	reRallyIcon->setPosition(270, 140);
	upgradePanel->addChild(reRallyIcon);

	preview = Sprite::createWithSpriteFrameName("tower_preview_archer.png");
	preview->setPositionY(30);
	preview->setOpacity(200);
	preview->setVisible(false);
	this->addChild(preview, 1);

	initRange();

	//ccBezierConfig con;
	//con.controlPoint_1 = Vec2(100, 100);
	//con.controlPoint_2 = Vec2(200, 200);
	//con.endPosition = Vec2(500, 500);
	//auto a1 = MoveBy::create(0.5f, Vec2(100, 100));
	//range->runAction(a1);
	//range->setPosition(range->getPosition()+Vec2(500,500));


	//tower = Sprite::create();
	//tower->setPositionY(20);
	//this->addChild(tower, 5);

	auto touch_listener = EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(BuildTerrain::callback_touch_began, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(BuildTerrain::callback_touch_ended, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, terrain);

	tower = nullptr;
	resetRallyReady = false;

	return true;
}

bool BuildTerrain::callback_touch_began(Touch * touch, Event * event)
{
	return true;
}

void BuildTerrain::callback_touch_ended(Touch * touch, Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto pos = target->convertTouchToNodeSpace(touch);
	auto touchInWindow = touch->getLocation();
	auto size = target->getContentSize();
	auto rect = Rect(0, 0, size.width, size.height);
	//auto rect = Rect(0, 0, 255, 255);

	//还没有建造,显示建造菜单
	if (!isBuilt) {
		//触摸2次，建塔
		if (isBuildReady()) {
			preview->setVisible(false);
			range->setVisible(false);
			rallyRange->setVisible(false);
			hideMenu();
			buildTower();
		}
		//未触摸
		else if (!isIconSelected()) {
			preview->setVisible(false);
			if (rect.containsPoint(pos)) {
				if (isMenuShown) {
					hideMenu();
				}
				else {
					showMenu();
				}
			}
			else {
				hideMenu();
			}
		}
		//触摸一次，显示preview
		else if (isIconSelected() && isMenuShown) {
			switch (selectedType)
			{
			case ICON_TYPE_ARCHER:
				preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_archer.png"));
				range->setScale(1.25);
				rallyRange->setVisible(false);
				range->setVisible(true);
				break;
			case ICON_TYPE_BARRACKS:
				preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_barracks.png"));
				range->setVisible(false);
				rallyRange->setVisible(true);
				break;
			case ICON_TYPE_MAGE:
				preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_mage.png"));
				range->setScale(float(1));
				rallyRange->setVisible(false);
				range->setVisible(true);
				break;
			case ICON_TYPE_ARTILLERY:
				preview->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tower_preview_artillery.png"));
				range->setScale(float(1));
				rallyRange->setVisible(false);
				range->setVisible(true);
				break;
			default:
				break;
			}
			preview->setVisible(true);
		}
	}
	//已经建造，显示升级菜单
	else {
		//触摸2次，升级或拆毁
		if (isBuildReady()) {
			if (buildType == ICON_TYPE_UPGRADE) {

				switch (towerType)
				{
				case 1:
					tower->removeFromParent();
					tower = nullptr;
					tower = ArcherTowerLv2::createTower();
					tower->setPositionY(20);
					this->addChild(tower, 5);
					break;
				case 2:
					tower->upgrade();
					break;
				case 3:
					tower->upgrade();
					break;
				case 4:
					tower->upgrade();
					break;
				default:
					break;
				}
				hideMenu();
			}
			//销毁
			else if (buildType == ICON_TYPE_SELL) {
				if (tower) {
					tower->removeFromParent();
					tower = nullptr;
					hideMenu();
					isBuilt = false;
				}
			}
		}
		//触摸一次，兵营重新设置集合点
		else if (isIconSelected() && isMenuShown) {
			if (selectedType == ICON_TYPE_RERALLY) {
				auto scaleto = ScaleTo::create(0.1f, 0.0f);
				upgradePanel->runAction(scaleto);
				resetRallyReady = true;
			}
		}
		//未触摸
		else if (!isIconSelected()) {
			//重设rally
			if (resetRallyReady) {
				auto bg = GameController::getInstance()->getBg();
				auto deltax = bg->getContentSize().width / 2 - bg->getPosition().x;
				auto deltay = bg->getContentSize().height / 2 - bg->getPosition().y;
				auto pos = this->getPosition() - Vec2(deltax, deltay);
				auto delta = touchInWindow - pos;
				if (delta.distance(Vec2(0, 0)) < 160) {
					auto touchInWindow = tower->convertTouchToNodeSpace(touch)+Vec2(0,35);
					tower->setRallyPos(touchInWindow);
					//确认反馈
					auto confirmFeedback1 = Sprite::createWithSpriteFrameName("confirm_feedback_layer2_0001.png");
					//auto confirmFeedback2 = Sprite::createWithSpriteFrameName("confirm_feedback_layer1_0001.png");
					//confirmFeedback1->setAnchorPoint(Vec2(0.5, 0.5));
					confirmFeedback1->setPosition(touchInWindow-Vec2(0,20));
					//confirmFeedback1->addChild(confirmFeedback2);
					this->addChild(confirmFeedback1);
					confirmFeedback1->runAction(Sequence::create(FadeOut::create(0.5f), DelayTime::create(0.3f),
						CallFunc::create([=]()
					{
						confirmFeedback1->removeFromParent();
					}),nullptr));
				}
				
				rallyRange->setVisible(false);
				range->setVisible(false);
				resetRallyReady = false;
				isMenuShown = false;
			}
			else {
				if (rect.containsPoint(pos)) {
					if (isMenuShown) {
						hideMenu();
					}
					else {
						if (towerType == ICON_TYPE_BARRACKS) {
							float range = tower->getRange();
							rallyRange->setScale(range*1.0 / 160);
							rallyRange->setVisible(true);
							reRallyIcon->setVisible(true);
						}
						else {
							float r = tower->getRange();
							range->setScale(r*1.0 / 160);
							range->setVisible(true);
							reRallyIcon->setVisible(false);
						}
						showMenu();
					}
				}
				else {
					rallyRange->setVisible(false);
					range->setVisible(false);
					hideMenu();
				}
			}
		}
	}

}

void BuildTerrain::showMenu()
{
	//buildPanel->inAnimation();
	//buildPanel->setVisible(true);
	//panel->setPosition(terrain->getPosition());
	//terrain->addChild(buildPanel);
	//建造菜单
	if (!isBuilt) {
		auto scaleto = ScaleTo::create(0.1f, 0.6f);
		buildPanel->runAction(scaleto);
	}
	//升级菜单
	else {
		auto scaleto = ScaleTo::create(0.1f, 0.6f);
		upgradePanel->runAction(scaleto);
	}
	isMenuShown = true;
}

void BuildTerrain::hideMenu()
{
	//buildPanel->outAnimation();
	//buildPanel->setVisible(false);
	if (!isBuilt) {
		auto scaleto = ScaleTo::create(0.1f, 0.0f);
		buildPanel->runAction(scaleto);
	}
	else {
		auto scaleto = ScaleTo::create(0.1f, 0.0f);
		upgradePanel->runAction(scaleto);
	}
	isMenuShown = false;
	range->setVisible(false);
	rallyRange->setVisible(false);
}

bool BuildTerrain::isIconSelected()
{
	for (int i = 0; i < 4; i++) {
		if (icons[i]->isSelected()) {
			selectedType = i + 1;
			return true;
		}
	}
	if (upgradeIcon->isSelected()) {
		selectedType = 5;
		return true;
	}
	if (sellIcon->isSelected()) {
		selectedType = 6;
		return true;
	}
	if (reRallyIcon->isSelected()) {
		selectedType = 7;
		return true;
	}
	return false;
}

bool BuildTerrain::isBuildReady()
{
	for (int i = 0; i < 4; i++) {
		if (icons[i]->isDoubleSelected()) {
			buildType = i + 1;
			return true;
		}
	}
	if (upgradeIcon->isDoubleSelected()) {
		buildType = 5;
		return true;
	}
	if (sellIcon->isDoubleSelected()) {
		buildType = 6;
		return true;
	}
	if (reRallyIcon->isDoubleSelected()) {
		buildType = 7;
		return true;
	}
	return false;
}

void BuildTerrain::buildTower()
{
	Sprite* building = Sprite::create();
	Sprite* buildBar = Sprite::createWithSpriteFrameName("buildbar.png");
	Sprite* buildBarBg = Sprite::createWithSpriteFrameName("buildbar_bg.png");
	std::string  tower_type;
	buildBar->setScaleX(0);
	buildBar->setScaleY(0.7);
	buildBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	buildBar->setPosition(-30, 50);
	buildBarBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	buildBarBg->setPosition(-30, 50);
	this->addChild(buildBarBg, 5);
	this->addChild(buildBar, 5);
	//显示建造中图案
	switch (buildType) {
	case 1:
		building->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("build_archer.png"));
		break;
	case 2:
		building->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("build_barracks.png"));
		break;
	case 3:
		building->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("build_mage.png"));
		break;
	case 4:
		building->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("build_artillery.png"));
		break;
	};
	building->setPositionY(30);
	this->addChild(building, 5);
	//进度条
	auto process = ScaleTo::create(0.5, 1, 0.7);
	//回调函数
	auto build = [=]() {
		buildBarBg->setVisible(false);
		buildBar->setVisible(false);
		building->setVisible(false);
		switch (buildType) {
		case ICON_TYPE_ARCHER:
			tower = ArcherTower::createTower();
			break;
		case ICON_TYPE_BARRACKS:
			tower = BarrackTower::createTower(rallyPos);
			break;
		case ICON_TYPE_ARTILLERY:
			tower = ArtilleryTower::createTower();
			break;
		case ICON_TYPE_MAGE:
			tower = MageTower::createTower();
			break;
		};

		tower->setPositionY(20);
		this->addChild(tower, 5);
		log("%s", typeid(tower).name());
		//tower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(tower_type));
		//this->removeChild(building, true);
		//this->removeChild(buildBarBg, true);
		isBuilt = 1;
		towerType = buildType;
	};
	buildBar->runAction(Sequence::create(process, CallFunc::create(build), nullptr));
}

void BuildTerrain::update(float dt)
{
	//if (isBuilt)
	//	tower->update(dt);
}

void BuildTerrain::initRange()
{
	range = Sprite::create();
	range->setPosition(preview->getPosition());
	auto range1 = Sprite::createWithSpriteFrameName("range_circle.png");
	auto range2 = Sprite::createWithSpriteFrameName("range_circle.png");
	auto range3 = Sprite::createWithSpriteFrameName("range_circle.png");
	auto range4 = Sprite::createWithSpriteFrameName("range_circle.png");
	range1->setPosition(-80, 80);
	range2->setPosition(80, 80);
	range3->setPosition(80, -80);
	range4->setPosition(-80, -80);
	range2->setRotation(90);
	range3->setRotation(180);
	range4->setRotation(270);
	range->addChild(range1);
	range->addChild(range2);
	range->addChild(range3);
	range->addChild(range4);
	range->setVisible(false);
	this->addChild(range, 1);

	rallyRange = Sprite::create();
	rallyRange->setPosition(preview->getPosition());
	auto rallyRange1 = Sprite::createWithSpriteFrameName("rally_circle.png");
	auto rallyRange2 = Sprite::createWithSpriteFrameName("rally_circle.png");
	auto rallyRange3 = Sprite::createWithSpriteFrameName("rally_circle.png");
	auto rallyRange4 = Sprite::createWithSpriteFrameName("rally_circle.png");
	rallyRange1->setPosition(-80, 80);
	rallyRange2->setPosition(80, 80);
	rallyRange3->setPosition(80, -80);
	rallyRange4->setPosition(-80, -80);
	rallyRange2->setRotation(90);
	rallyRange3->setRotation(180);
	rallyRange4->setRotation(270);
	rallyRange->addChild(rallyRange1);
	rallyRange->addChild(rallyRange2);
	rallyRange->addChild(rallyRange3);
	rallyRange->addChild(rallyRange4);
	rallyRange->setVisible(false);
	this->addChild(rallyRange, 1);
}
