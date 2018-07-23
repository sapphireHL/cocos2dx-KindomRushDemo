#include "ArtilleryTower.h"

ArtilleryTower * ArtilleryTower::createTower(int level)
{
	auto tower = new ArtilleryTower();
	if (tower && tower->init(level)) {
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return nullptr;
}

bool ArtilleryTower::init(int level)
{
	if (!Sprite::init()) {
		return false;
	}

	base = Sprite::createWithSpriteFrameName("artillery_base_000" + std::to_string(level) + ".png");
	thrower = Sprite::createWithSpriteFrameName("artillery_thrower_0001.png");
	thrower->setPosition(0, 15);
	this->addChild(base);
	this->addChild(thrower);

	return true;
}

void ArtilleryTower::shoot()
{
}

void ArtilleryTower::update(float dt)
{
}

bool ArtilleryTower::searchMonster()
{
	return false;
}
