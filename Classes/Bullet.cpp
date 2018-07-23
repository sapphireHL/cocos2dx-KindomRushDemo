#include "Bullet.h"

Bullet * Bullet::create()
{
	auto bullet = new Bullet;
	if (bullet && bullet->init()) {
		bullet->autorelease();
		return bullet;
	}
	delete bullet;
	bullet = nullptr;
	return nullptr;
}

bool Bullet::init()
{
	if (!Sprite::init())
		return false;
	return true;	
}

void Bullet::setAction(Spawn *a)
{
	action = a;
}

void Bullet::setFlip()
{
	bulletSprite->setFlippedX(true);
}

Vec2 Bullet::getBulletPosition()
{
	return bulletSprite->getPosition();
}

void Bullet::setTarget(Monster * t)
{
	target = t;
}

void Bullet::setPower(int pow)
{
	power = pow;
}
