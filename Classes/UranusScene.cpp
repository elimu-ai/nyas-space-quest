//
//  UranusScene.cpp
//  akua
//
//  Created by German Torres on 12/6/15.
//
//

#include "UranusScene.h"
using namespace CocosDenshion;

Scene * Uranus::createScene()
{
    auto scene = Scene::create();
    auto layer = Uranus::create();
    scene->addChild(layer);
    layer->loadMap();
    return scene;
}

bool Uranus::init()
{
    if (!GameMap::init())
    {
        return false;
    }
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("uranus.plist");
	cache->addSpriteFramesWithFile("uranusTips.plist");
    return true;
}

void Uranus::loadMap()
{
    setupTilemap("uranusLevel.tmx");
	snow = ParticleSnow::create();
	snow->setPositionType(ParticleSystem::PositionType::RELATIVE);
	snow->setPosVar(Vec2(winSize.width*2, 200));
	snow->setEndSpin(200);
	snow->setEndSpinVar(-360);
	auto cache = SpriteFrameCache::getInstance();
	auto * spFrame = cache->getSpriteFrameByName("iceParticle.png");
	snow->setTextureWithRect(spFrame->getTexture(), spFrame->getRect());
	gameplayNode->addChild(snow, 10100);
	setupBackground();
	setupParallaxImages();
	setupSpikes();
    GameMap::loadMap();
}

void Uranus::setupBackground()
{
	auto bgSprite = Sprite::createWithSpriteFrameName("bg.png");
	bgSprite->setAnchorPoint(Vec2::ZERO);
	addChild(bgSprite, -10);
	auto starsFX = Sprite::createWithSpriteFrameName("stars.jpg");
	starsFX->setAnchorPoint(Vec2::ZERO);
	starsFX->setBlendFunc(BlendFunc::ADDITIVE);
	starsFX->setOpacity(200);
	addChild(starsFX, -10);
}

void Uranus::setupParallaxImages()
{
	for (int i = 1; i <= 3; ++i)
	{
		std::string name = "p" + std::to_string(i) + ".png";
		auto sprite = Sprite::createWithSpriteFrameName(name);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPositionX((i - 1)*sprite->getContentSize().width);
		parallaxNodeBack->addChild(sprite);
	}
}

void Uranus::setupSpikes()
{
	auto spikes = tiledMap->getObjectGroup("spikes")->getObjects();
	for (auto spike : spikes)
	{
		auto spikeMap = spike.asValueMap();
		int x = spikeMap["x"].asInt();
		int y = spikeMap["y"].asInt();
		auto object = Spike::create();
		object->setPosition(Vec2(x, y));
		object->setAnchorPoint(Vec2(0.5, 0));
		object->setScale(0.3f);
		spikeVector.pushBack(object);
		gameplayNode->addChild(object, 100);
	}
}

void Uranus::update(float dt)
{
	//spikes
	for (Spike * spike : spikeVector)
	{
		if (player->checkIntersect(spike))
		{
			this->hurtPlayer();
		}
	}
	snow->setPosition(player->getPosition() + Vec2(0, 3 * winSize.height / 4));

	GameMap::update(dt);
}
