#include "SkillItem.h"

bool SkillItem::init()
{
	if (!Sprite::init())
		return false;

	return true;
}

void SkillItem::applySkill(Vec2 pos)
{
}

void SkillItem::cool(float dt)
{
}

void SkillItem::select()
{
	if (itemSelected) {
		itemSelected->setVisible(true);
	}
}

void SkillItem::unselect()
{
	if (itemSelected) {
		itemSelected->setVisible(false);
	}
}

bool SkillItem::selected()
{
	if(itemSelected)
		return itemSelected->isVisible();
}
