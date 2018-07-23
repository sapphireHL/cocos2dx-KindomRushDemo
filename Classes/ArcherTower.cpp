#include "ArcherTower.h"
#include "Arrow.h"
#include "GameController.h"

ArcherTower * ArcherTower::createTower(int level)
{
	auto tower = new ArcherTower();
	if (tower && tower->init(level)) {
		tower->setType(1);
		tower->setRange(200);
		tower->setPower(5);
		tower->autorelease();
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return nullptr;
}

bool ArcherTower::init(int level)
{
	if (!Sprite::init()) {
		return false;
	}
	base = Sprite::createWithSpriteFrameName("archer_towers_000" + std::to_string(level)+".png");
	shooter = Sprite::createWithSpriteFrameName("archer_shooter_0009.png");
	shooter->setPosition(0, 15);
	//auto shoot = AnimationCache::getInstance()->getAnimation("archer_shooter_attack");
	//shooter->runAction(RepeatForever::create(Animate::create(shoot)));
	//shoot();
	this->addChild(base);
	this->addChild(shooter);
	monsterOrientation = 0;

	schedule(schedule_selector(ArcherTower::update), 0.8f);
	return true;
}

void ArcherTower::shoot()
{
	if (nearestMonster->getNextPosition() == ERROR_POSITION)
		return;
	auto targetpos = nearestMonster->getPosition();//getRoute()[nearestMonster->curpos];// -Vec2(0, nearestMonster->getSize().y / 2);
	//auto draw = DrawNode::create();
	//this->addChild(draw,9);
	//draw->drawDot(monsterpos - this->getParent()->getPosition(), 10, Color4F::BLACK);
	//log("mons:%f, %f\n", monsterpos.x, monsterpos.y);
	//创建箭
	arrow = Arrow::creat();
	arrow->setFlippedX(true);
	this->addChild(arrow);
	arrow->setPosition(0, 20);
	
	//设置箭的动作
	auto v1 = arrow->getPosition() + this->getPosition() + this->getParent()->getPosition();
	auto delta = targetpos + correctShoot(nearestMonster->getState()) - v1;
	//auto draw = DrawNode::create();
	//this->addChild(draw);
	//draw->drawLine(arrow->getPosition(), arrow->getPosition() + delta, Color4F::BLACK);
	shootPos = delta;// monsterpos - this->getParent()->getPosition() - this->getPosition();
	//计算箭到敌人的角度
	float radians = atan2(delta.y, delta.x);
	if (radians < 0) radians += 2 * 3.1415926;
	float angle = radians / 3.1415926 * 180;
	float shootAngle = angle;
	log("angle:%f\n", angle);
	arrow->setRotation(-shootAngle);//设置弓箭角度
	ccBezierConfig con;
	con.controlPoint_1 = Vec2(arrow->getPosition().x, arrow->getPosition().y + 50);
	con.controlPoint_2 = Vec2(shootPos.x, shootPos.y + 50);
	con.endPosition = shootPos;
	auto action = BezierBy::create(0.4f, con);
	auto action1 = EaseInOut::create(action, 0.5);
	RotateTo* rotate;
	//选择朝向
	switch (monsterOrientation)
	{
	case LEFT:
		shooter->setFlippedX(true);
		//arrow->setFlip();
		if(shootAngle > 270)
			rotate = RotateTo::create(0.4f, -shootAngle);
		else
			rotate = RotateTo::create(0.4f, -225);
		break;
	case RIGHT:
		shooter->setFlippedX(false);
		rotate = RotateTo::create(0.4f, 45);
		break;
	default:
		break;
	}
	//弓箭手动作
	auto shoot = AnimationCache::getInstance()->getAnimation("archer_shooter_attack");
	shooter->runAction(Animate::create(shoot));
	//auto bulletSprite = Sprite::createWithSpriteFrameName("arrow.png");
	//bulletSprite->setFlippedX(true);
	//bulletSprite->setPosition(shooter->getContentSize().width / 2, shooter->getContentSize().height / 2);
	//shooter->addChild(bulletSprite);
	//bulletSprite->runAction(MoveBy::create(0.1f, Vec2(100, 100)));


	//auto action1 = MoveBy::create(0.3f, Vec2(100, 100));
	
	//箭动作
	auto action2 = EaseIn::create(rotate, 0.9);
	arrow->setAction(Spawn::create(action1, rotate, nullptr));
	arrow->setTarget(nearestMonster);
	arrow->setPower(power);
	arrow->shoot();
	//arrow->setPosition(shootPos);
	auto rea2 = arrow->getPosition() + this->getParent()->getPosition();
	//log("arrow:%f, %f\n", rea2.x, rea2.y);
	//arrow->setPosition(arrow->getPosition() + shootPos);
	//if (hitOnMonster()) {
	//	nearestMonster->setCurHP(nearestMonster->getCurHP() - 10);
	//}
}

void ArcherTower::update(float dt)
{
	if (searchMonster() && nearestMonster->getCurHP() > 0) {
		shoot();
	}
	else {
		nearestMonster = nullptr;
	}
}

bool ArcherTower::searchMonster()
{
	//若与当前目标距离太远或目标死亡，就清0准备找新目标
	if (nearestMonster) {
		if (nearestMonster->isDead())
			nearestMonster = nullptr;
		else {
			auto selfpos = this->getParent()->getPosition(), targetpos = nearestMonster->getPosition();
			if (ccpDistance(selfpos, targetpos) > range) {
				nearestMonster = nullptr;
			}
		}
	}
	
	auto monsters = GameController::getInstance()->getCurrentWaveMonsters();
	for (int i = 0; i < (int)monsters.size(); i++) {
		if (monsters[i]->isDead())
			continue;
		auto vec1 = this->getParent()->getPosition(), vec2 = monsters[i]->getPosition();
		auto vec3 = this->getPosition();
		float dis = ccpDistance(vec1, vec2);
		if (dis <= range) {
			//shootPos = vec2 - vec1;
			if(!nearestMonster)
				nearestMonster = monsters[i];
			//方位
			if (vec2.x > vec1.x && vec2.y > vec1.y)
				monsterOrientation = RIGHT;// (RIGHT | UP);
			else if (vec2.x < vec1.x && vec2.y > vec1.y)
				monsterOrientation = LEFT;// (LEFT | UP);
			else if (vec2.x > vec1.x && vec2.y < vec1.y)
				monsterOrientation = RIGHT;// (RIGHT | DOWN);
			else if (vec2.x < vec1.x && vec2.y < vec1.y)
				monsterOrientation = LEFT;// (LEFT | DOWN);
			return true;
		}
	}
	nearestMonster = nullptr;
	return false;
}

bool ArcherTower::hitOnMonster()
{
	auto v1 = arrow->getBulletPosition() + this->getParent()->getPosition(), v2 = nearestMonster->getPosition();
	//log("arrow:%f,%f\n", v1.x, v1.y);
	//log("monster:%f,%f\n", v2.x, v2.y);
	if (ccpDistance(v1, v2) < 20) {
		return true;
	}
	return false;
}

Vec2 ArcherTower::correctShoot(MonsterState st)
{
	switch (st)
	{
	case stateWalkRight:
		return Vec2(20, 0);
	case stateWalkLeft:
		return Vec2(-20, 0);
	case stateWalkUp:
		return Vec2(0, 20);
	case stateWalkDown:
		return Vec2(0, -20);
	case stateAttack:
		return Vec2();
	default:
		return Vec2();
	}
	
}
