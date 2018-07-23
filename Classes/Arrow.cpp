#include "Arrow.h"

Arrow * Arrow::creat()
{
	auto arr = new Arrow;
	if (arr && arr->init()) {
		arr->autorelease();
		return arr;
	}
	delete arr;
	arr = nullptr;
	return nullptr;
}

bool Arrow::init()
{
	if (!Sprite::init())
		return false;
	
	bulletSprite = Sprite::createWithSpriteFrameName("arrow.png");
	bulletSprite->setFlippedX(true);
	//bulletSprite->setRotation(-45);
	this->addChild(bulletSprite);

	return true;
}

void Arrow::shoot() {
	auto removeArrow = [=]() {
		this->removeFromParent();
	};
	auto hitOn = [=]() {
		auto bpos = bulletSprite->getPosition() + this->getParent()->getPosition() + this->getParent()->getParent()->getPosition();
		log("arrow:%f,%f\n", bpos.x, bpos.y);
		log("target:%f,%f\n", target->getPosition().x, target->getPosition().y);
		bpos += Vec2(0, 20);
		//if (ccpDistance(bpos, target->getPosition()) < 20) {
			target->getHurt();
			target->declineHp(power);
			//this->removeFromParent();
		//}
	};
	//发射完后消失
	auto v1 = this->getPosition() + this->getParent()->getPosition() + this->getParent()->getParent()->getPosition();
	bulletSprite->runAction(Sequence::create(action, CallFunc::create(hitOn), DelayTime::create(0.2f), CallFunc::create(removeArrow), nullptr));
}

void Arrow::setFlip()
{
	bulletSprite->setRotation(-135);
}

void Arrow::setRotation(float r)
{
	bulletSprite->setRotation(r);
}

void Arrow::calcRoute(Vec2 delta)
{
	float angle = delta.getAngle() / 3.1415926 * 180;
	this->setRotation(-angle);//设置弓箭角度
	ccBezierConfig con;
	//con.controlPoint_1 = Vec2(0.22*(delta.x), 0.99*abs(delta.y));
	//con.controlPoint_2 = Vec2( 0.22*(delta.x), 1*abs(delta.y));
	con.controlPoint_1 = delta.x > 0 ? Vec2(50, 50) : Vec2(-50, 50);
	con.controlPoint_2 = delta;
	con.endPosition = delta;
	auto move = BezierBy::create(0.5f, con);
	//旋转
	RotateTo* rotate;
	if (delta.x > 0) {
		this->setFlippedX(false);
		rotate = RotateTo::create(0.5f, 45);
	}
	else {
		this->setFlippedX(true);
		if (angle + 360 > 270)
			rotate = RotateTo::create(0.5f, angle);
		else
			rotate = RotateTo::create(0.5f, -225);
	}
	this->setAction(Spawn::create(move, rotate, nullptr));
}
