#include "Bot.h"

#define kDistanceX 80.0f
#define kDistanceY 100
#define kHorizontalSpeed 3.5

USING_NS_CC;

Bot * Bot::create()
{
	Bot * bot = new Bot();

	if (bot && bot->init())
	{
		bot->autorelease();
		bot->initBot();
		return bot;
	}
	CC_SAFE_DELETE(bot);
	return NULL;
}

Bot::~Bot() {}

void Bot::initBot()
{
	botSprite = Sprite::createWithSpriteFrameName("botProfile.png");
	auto moveBy = MoveBy::create(1, Vec2(0, 3));
	auto moveBack = moveBy->reverse();
	auto seq = Sequence::create(moveBy, DelayTime::create(0.3f), moveBack, DelayTime::create(0.2f), nullptr);
	botSprite->runAction(RepeatForever::create(seq));
	this->addChild(botSprite);
	bubbleModeEnabled = false;
}

void Bot::update(Vec2 playerPosition, float dt)
{
	auto cache = SpriteFrameCache::getInstance();
	if (t1 <= 0)
	{
		t1 = RandomHelper::random_real(16.0f, 30.0f);
		t2 = RandomHelper::random_real(0.5f, 2.1f);
	}
	else
	{
		t1 -= dt;
	}
	if (t2 >= 0 && !isOpen)
	{
		isOpen = true;
		botSprite->setSpriteFrame(cache->getSpriteFrameByName("botProfileOpen.png"));
	}
	if (t2 <= 0 && isOpen)
	{
		isOpen = false;
		botSprite->setSpriteFrame(cache->getSpriteFrameByName("botProfile.png"));
	}
	if (t2 >= 0)
	{
		t2 -= dt;
	}

	if (bubbleModeEnabled)
	{
		this->setPosition(playerPosition + Vec2(0, 60));
		return;
	}
	float dx = this->getPositionX() - playerPosition.x;
	float dy = this->getPositionY() - playerPosition.y;
	Vec2 dPos = Vec2(dx, dy);
	setBotPosition(dt, dPos);
}

void Bot::bubbleMode(Vec2 playerPosition)
{
	auto cache = SpriteFrameCache::getInstance();
	botSprite->setSpriteFrame(cache->getSpriteFrameByName("botProfile.png"));
	isOpen = false;
	auto moveBot = MoveTo::create(0.1, playerPosition + Vec2(0, 60));
	auto growBot = ScaleTo::create(0.1, 2.55);
	auto changeMode = CallFunc::create([&]() {
		bubbleModeEnabled = true;
	});
	auto seqBot = Sequence::create(moveBot, growBot, changeMode, NULL);
	botSprite->setOpacity(100);
	this->runAction(seqBot);
}

void Bot::normalMode()
{
	auto shrinkBot = ScaleTo::create(0.4, 1);
	this->runAction(shrinkBot);
	botSprite->setOpacity(255);
	bubbleModeEnabled = false;
}

void Bot::setBotPosition(float dt, Vec2 dPos)
{
	setBotXPosition(dt, dPos.x);
	setBotYPosition(dt, dPos.y);
}

void Bot::setBotXPosition(float dt, float dx)
{
	if (dx > kDistanceX)
	{
		this->setPositionX(this->getPositionX() - kHorizontalSpeed*dt*(dx - kDistanceX));
		botSprite->setFlippedX(false);
	}
	else if (dx < -kDistanceX)
	{
		this->setPositionX(this->getPositionX() - kHorizontalSpeed*dt*(dx + kDistanceX));
		botSprite->setFlippedX(true);
	}

	if (dx > kDistanceX + 5)
	{
		this->setRotation(-7);
	}
	else if (dx < -kDistanceX - 5)
	{
		this->setRotation(7);
	}
	else
	{
		this->setRotation(0);
	}
}

void Bot::setBotYPosition(float dt, float dy)
{
	if (dy > kDistanceY)
	{
		this->setPositionY(this->getPositionY() - dt*(dy - kDistanceY));
	}
	else if (dy < kDistanceY)
	{
		this->setPositionY(this->getPositionY() - dt*(dy - kDistanceY));
	}
}