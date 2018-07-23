#pragma once
#include "cocos2d.h"

using namespace cocos2d;

const int TAG_DELETE = 101;
const int TAG_NEWGAME = 102;

class SaveSlot : public Sprite {
	SaveSlot();
	~SaveSlot();

public:
	virtual bool initWithNum(int num);
	static SaveSlot* createSlot(int num);
	CREATE_FUNC(SaveSlot);

private:
	void init_button_slot_delete();
	void init_delete_menu();
	void showRecord();

	void deleteRecord(int num);
	void createNewGame();
	void callback_slot_delete(Ref* pSender);
	void callback_confirm_delete(Ref* pSender);
	void callback_cancel_delete(Ref* pSender);

	CC_SYNTHESIZE(int, num, Num);
protected:
	Sprite* slot;
	MenuItemSprite* button_slot_delete;
};