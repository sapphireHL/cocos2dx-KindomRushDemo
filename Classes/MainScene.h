#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class MainScene : public Scene {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);
};