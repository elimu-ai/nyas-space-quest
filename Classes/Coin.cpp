//
//  Coin.cpp
// Nya
//
//  Created by German Torres on 8/3/15.
//
//

#include "Coin.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Coin * Coin::create()
{
	Coin * coin = new Coin();

	if (coin && coin->init())
	{
		coin->autorelease();
		coin->initCoin();
		return coin;
	}
	CC_SAFE_DELETE(coin);
	return NULL;
}

Coin::~Coin()
{
	disappear->release();
}

void Coin::initCoin()
{
	setupBoundary();
	setupCoin();
	setupAudio();
	BaseObject::initObject();
}

void Coin::setupBoundary()
{
	boundary.shape = SHAPE::circle;
	boundary.active = true;
	boundary.r = 25;
}

void Coin::setupCoin()
{
	consumed = false;
	coinSprite = Sprite::createWithSpriteFrameName("coin.png");
	auto rotation = RotateBy::create(RandomHelper::random_int(1, 4), 25);
	coinSprite->runAction(RepeatForever::create(rotation));
	coinSprite->setOpacity(140);
	disappear = Sequence::create(ScaleTo::create(0.3, 1.9), FadeOut::create(0.5), nullptr);
	disappear->retain();
	this->addChild(coinSprite);
	this->setScale(0.7);
}

void Coin::setupAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/coin.wav");
}

void Coin::consume()
{
	consumed = true;
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("sfx/coin.wav");
	coinSprite->runAction(disappear);
}
