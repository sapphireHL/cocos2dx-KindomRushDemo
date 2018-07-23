#include "AnimationManager.h"

AnimationManager* AnimationManager::instance;

AnimationManager * AnimationManager::getInstance()
{
	if (instance == nullptr)
		instance = new AnimationManager;
	return instance;
}

void AnimationManager::init_woods()
{
	createAnimationFromFile("gnoll_reaver_animations.plist", 0.05f);
	createAnimationFromFile("gnoll_burner_animations.plist", 0.05f);
	createAnimationFromFile("gnoll_gnawer_animations.plist", 0.05f);
}

void AnimationManager::init_archer()
{
	Vector<SpriteFrame*> frames;
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("archer_shooter_0001.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("archer_shooter_0004.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("archer_shooter_0006.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("archer_shooter_0008.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("archer_shooter_0009.png");
	frames.pushBack(frame);
	auto animation = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(animation, "archer_shooter_attack");


}

void AnimationManager::init_barrack()
{
	//塔门开关
	Vector<SpriteFrame*> frames;
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("barracks_towers_layer2_0001.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("barracks_towers_layer2_0002.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("barracks_towers_layer2_0003.png");
	frames.pushBack(frame);
	auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(animation, "barrack_open_door");
	frames.reverse();
	auto animation2 = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(animation2, "barrack_close_door");
	//士兵行走lv1
	frames.clear();
	for (int i = 1; i <= 6; ++i) {
		std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0001.png");
	frames.pushBack(frame);
	auto walkAnimationLv1 = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(walkAnimationLv1, "soldier_walkingRightLv1");
	//士兵攻击lv1
	frames.clear();
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0007.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0011.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0014.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0017.png");
	frames.pushBack(frame);
	//for (int i = 7; i <= 17; ++i) {
	//	std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
	//	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
	//	frames.pushBack(frame);
	//}
	auto attackAnimationLv1 = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(attackAnimationLv1, "soldier_attackLv1");
	//士兵死亡lv1
	frames.clear();
	for (int i = 23; i <= 29; ++i) {
		std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	auto deathAnimationLv1 = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(deathAnimationLv1, "soldier_deathLv1");
	//行走Lv2
	frames.clear();
	for (int i = 32; i <= 37; ++i) {
		std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0032.png");
	frames.pushBack(frame);
	auto walkAnimationLv2 = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(walkAnimationLv2, "soldier_walkingRightLv2");
	//攻击lv2
	frames.clear();
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0038.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0040.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0042.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0045.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0046.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0047.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("soldiers_123_0048.png");
	frames.pushBack(frame);
	//for (int i = 38; i <= 47; ++i) {
	//	std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
	//	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
	//	frames.pushBack(frame);
	//}
	auto attackAnimationLv2 = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(attackAnimationLv2, "soldier_attackLv2");
	//死亡lv2
	frames.clear();
	for (int i = 64; i <= 71; ++i) {
		std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	auto deathAnimationLv2 = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(deathAnimationLv2, "soldier_deathLv2");
	//射击lv2
	frames.clear();
	for (int i = 48; i <= 59; ++i) {
		std::string frameName = String::createWithFormat("soldiers_123_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	auto shootAnimationLv2 = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(shootAnimationLv2, "soldier_shootLv2");
}

void AnimationManager::init_artillery()
{
	createAnimationFromFile("artillery_thrower_animations.plist", 0.03f);
	//初始石头变化
	Vector<SpriteFrame*> frames;
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0001.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0003.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0005.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0007.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0009.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0011.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0013.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0015.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0017.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0019.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0021.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0023.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_stones_0025.png");
	frames.pushBack(frame);
	auto animation = Animation::createWithSpriteFrames(frames, 0.08f);
	AnimationCache::getInstance()->addAnimation(animation, "artillery_henge_chargeStone");
	//石头爆炸效果
	frames.clear();
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0001.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0003.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0005.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0007.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0009.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0011.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0013.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0015.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("artillery_thrower_explosion_0017.png");
	frames.pushBack(frame);
	auto exploAnimation = Animation::createWithSpriteFrames(frames, 0.08f);
	AnimationCache::getInstance()->addAnimation(exploAnimation, "artillery_thrower_explosion");
}

void AnimationManager::init_mage()
{
	createAnimationFromFile("mage_towers_animations.plist", 0.04f);
	createAnimationFromFile("mage_tower_shooter_animations.plist", 0.04f);
	//魔法球动作
	Vector<SpriteFrame*> frames;
	SpriteFrame* frame;
	for (int i = 1; i <= 15; ++i) {
		std::string frameName = String::createWithFormat("voodoo_proy_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	auto runAnimation = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(runAnimation, "voodoo_run");
	frames.clear();
	for (int i = 16; i <= 24; ++i) {
		std::string frameName = String::createWithFormat("voodoo_proy_%04d.png", i)->getCString();
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		frames.pushBack(frame);
	}
	auto exploAnimation = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(exploAnimation, "voodoo_explosion");
}

void AnimationManager::init_hero_archer()
{
	createAnimationFromFile("archer_hero_animations.plist", 0.05f);
}

void AnimationManager::init_skill()
{
	//闪电爆炸
	Vector<SpriteFrame*> frames;
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0001.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0003.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0005.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0007.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0009.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0011.png");
	frames.pushBack(frame);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_half_0013.png");
	frames.pushBack(frame);
	//auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0001.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0003.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0005.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0007.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0009.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0011.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0013.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0015.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0017.png");
	//frames.pushBack(frame);
	//frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ray_explosion_decal_0019.png");
	//frames.pushBack(frame);
	auto exploAnimation = Animation::createWithSpriteFrames(frames, 0.05f);
	AnimationCache::getInstance()->addAnimation(exploAnimation, "lightning_storm_explosion");
}

void AnimationManager::createAnimationFromFile(std::string file, float duration)
{
	auto mp = FileUtils::getInstance()->getValueMapFromFile(file);
	auto anims = mp.at("animations").asValueMap();
	for (auto obj : anims) {
		auto name = obj.first;
		auto map = obj.second.asValueMap();
		auto prefix = map.at("prefix").asString();
		auto fromIndex = map.at("fromIndex").asInt();
		auto toIndex = map.at("toIndex").asInt();
		Vector<SpriteFrame*> frames;
		for (int i = fromIndex; i <= toIndex; i++) {
			std::string frameName = String::createWithFormat("%s_%04d.png", prefix.c_str(), i)->getCString();
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
			frames.pushBack(frame);
		}
		auto animation = Animation::createWithSpriteFrames(frames, duration);
		AnimationCache::getInstance()->addAnimation(animation, name);
	}
}
