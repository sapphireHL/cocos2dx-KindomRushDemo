#include "MageTower.h"
#include "GameController.h"
#include "MagicBolt.h"
#include "MagicCurve.h"

MageTower * MageTower::createTower(int level)
{
	auto tower = new MageTower;
	if (tower && tower->init(level)) {
		tower->autorelease();
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return nullptr;
}

bool MageTower::init(int level)
{
	if (!Sprite::init())
		return false;

	terrain = Sprite::createWithSpriteFrameName("mage_towers_layer1_0001.png");
	base = Sprite::createWithSpriteFrameName("mage_towers_layer2_0001.png");
	magician = Sprite::createWithSpriteFrameName("mage_tower_shooter_0001.png");
	magician->setPosition(0, 25);
	this->addChild(terrain);
	this->addChild(base);
	this->addChild(magician);

	setType(3);
	setRange(160);
	setPower(8);

	this->level = level;

	//索敌频率
	schedule(schedule_selector(MageTower::update), 0.1f);

	return true;
}

void MageTower::shoot(bool down)
{
	//base的动画
	std::string baseAniname = String::createWithFormat("mage_towers_lvl%d_shoot", level)->getCString();
	auto baseAnimation = AnimationCache::getInstance()->getAnimation(baseAniname);
	base->runAction(Animate::create(baseAnimation));
	//shooter的动画
	std::string shootName = "mage_tower_shooter_shoot";
	if (down)
		shootName += "Down";
	else
		shootName += "Up";
	auto shootAnimation = AnimationCache::getInstance()->getAnimation(shootName);
	magician->runAction(Animate::create(shootAnimation));

	//子弹
	auto animation = AnimationCache::getInstance()->getAnimation("voodoo_run");
	for (int i = 0; i < 2; ++i) {
		MagicBolt* bolt = MagicBolt::creat();
		bolt->setPosition(-10, 30);
		bolt->setVisible(false);
		bolt->setPower(power);
		this->addChild(bolt);
		Monster* target = nullptr;
		if (nearMonsters.size() == 1) 
			target = nearMonsters[0];
		else
			target = nearMonsters[i];
		Vec2 shootPos;
		if (target->isAttacked())
			shootPos = target->getWorldPosition();
		else
			shootPos = target->getWorldPositionForecast(8);
		auto delta = shootPos - convertToWorldSpace(bolt->getPosition());
		//曲线
		MagicConfig con;
		con.endPostion = delta;
		con.heightRate = 0.5f;
		con.upside = i;
		con.period = 1.0f;
		auto move = MagicCurve::create(1.0f, con);
		//ccBezierConfig con;
		//con.controlPoint_1 = Vec2(bolt->getPosition().x + 0.27*(delta.x), bolt->getPosition().y + 0.82*abs(delta.y));
		//con.controlPoint_2 = Vec2(bolt->getPosition().x + 0.27*(delta.x), bolt->getPosition().y + 0.82*abs(delta.y));
		//con.endPosition = delta;
		//auto move = BezierBy::create(1.0f, con);
		bolt->setMove(move);
		//bolt->setAction(Spawn::create(Animate::create(animation), move, nullptr));
		bolt->setTarget(target);
		bolt->shoot();
	}
}

void MageTower::update(float dt)
{
	if (searchMonster()) {
		shoot(true);
		//射击频率
		schedule(schedule_selector(MageTower::update), 1.8f);
	}
	else {
		schedule(schedule_selector(MageTower::update), 0.1f);
	}
}

bool MageTower::searchMonster()
{
	nearMonsters.clear();
	auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
	auto myPos = convertToWorldSpace(this->getPosition());
	for (auto mons : monsters) {
		if (mons->isDead()) continue;
		auto monsterPos = mons->getWorldPosition();
		float dis = myPos.distance(monsterPos);
		if (dis < range) {
			nearMonsters.push_back(mons);
		}
	}
	return nearMonsters.size() > 0;
}

void MageTower::upgrade()
{
	//++level;
	level = 2;
	terrain->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mage_towers_layer1_0033.png"));
	base->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mage_towers_layer2_0033.png"));
	//terrain = Sprite::createWithSpriteFrameName("mage_towers_layer1_0033.png");
	//base = Sprite::createWithSpriteFrameName("mage_towers_layer2_0033.png");
	setRange(120+40*level);
	setPower(4+4*level);
}
