#include "BaseTower.h"

bool Tower::init()
{
	if (!Sprite::init())
		return false;
	return true;
}

