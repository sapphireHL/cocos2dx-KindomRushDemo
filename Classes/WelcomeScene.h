#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class WelcomeScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(WelcomeScene);

	void init_background();//����
	void init_logo();
	void init_startButton();//��ʼ��ť
	void init_save_menu();//��¼��
	void save_menu_slide_in();
	void save_menu_slide_out();

	void callback_start(Ref* pSender);
	void callback_close(Ref* pSender);

private:
	Vec2 position_logo;
	Sprite* menu_save;
};