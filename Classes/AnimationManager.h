#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class AnimationManager {
public:
	static AnimationManager* getInstance();
	void init_woods();
	void init_archer();
	void init_barrack();
	void init_artillery();
	void init_mage();
	void init_hero_archer();
	void init_skill();

	void createAnimationFromFile(std::string path, float duration);

private:
	static AnimationManager* instance;
};