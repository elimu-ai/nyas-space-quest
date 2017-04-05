//
//  MarsScene.cpp
// Nya
//
//  Created by German Torres on 12/9/15.
//
//

#include "MarsScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene * Mars::createScene()
{
	auto scene = Scene::create();
	auto layer = Mars::create();
	scene->addChild(layer);
	layer->loadMap();
	return scene;
}

bool Mars::init()
{
	if (!GameMap::init())
	{
		return false;
	}
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("mars.plist");
	cache->addSpriteFramesWithFile("marsTips.plist");
	return true;
}

void Mars::loadMap()
{
	setupBackground();
	setupParallaxImages();
	setupTilemap("marsLevel.tmx");
	GameMap::loadMap();
}

void Mars::setupBackground()
{
	dust = ParticleRain::create();
	dust->setPositionType(ParticleSystem::PositionType::RELATIVE);
	dust->setPosVar(Vec2(winSize.width * 1, 0));
	dust->setGravity(Vec2(-200, -40));
	dust->setStartColor(Color4F::RED);
	gameplayNode->addChild(dust, 10100);
	auto bgSprite = Sprite::createWithSpriteFrameName("bg.png");
	bgSprite->setAnchorPoint(Vec2::ZERO);
	addChild(bgSprite, -10);
	auto starsFX = Sprite::createWithSpriteFrameName("stars.jpg");
	starsFX->setAnchorPoint(Vec2::ZERO);
	starsFX->setBlendFunc(BlendFunc::ADDITIVE);
	starsFX->setOpacity(200);
	addChild(starsFX, -10);
	auto olympusSprite = Sprite::createWithSpriteFrameName("olympus.png");
	olympusSprite->setAnchorPoint(Vec2::ZERO);
	addChild(olympusSprite, -9);

	auto curiosity = Sprite::createWithSpriteFrameName("curiosity.png");
	curiosity->setAnchorPoint(Vec2(0.5, 0));
	curiosity->setScale(0.9f);
	curiosity->setPosition(2400, 50);
	curiosity->setTag(66);
	parallaxNodeBack->addChild(curiosity, 1);

	auto oportunity = Sprite::createWithSpriteFrameName("oportunity.png");
	oportunity->setAnchorPoint(Vec2(0.5, 0));
	oportunity->setScale(0.9f);
	oportunity->setPosition(3800, 65);
	oportunity->setTag(66);
	parallaxNodeBack->addChild(oportunity, 1);
}

void Mars::setupParallaxImages()
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


void Mars::update(float dt)
{
	dust->setPosition(player->getPosition() + Vec2(0, 3 * winSize.height / 4));
	GameMap::update(dt);
}