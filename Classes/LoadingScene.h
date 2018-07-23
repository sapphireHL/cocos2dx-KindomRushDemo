#pragma once
#include "cocos2d.h"

class LoadingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoadingScene);

	void loadSource();
	void update(float delta) override;

private:
	int loaded;
};