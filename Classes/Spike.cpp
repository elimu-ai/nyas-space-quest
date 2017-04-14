#include "Spike.h"


USING_NS_CC;

Spike * Spike::create()
{
	Spike * spike = new Spike();

	if (spike && spike->init())
	{
		spike->autorelease();
		spike->initSpike();
		return spike;
	}
	CC_SAFE_DELETE(spike);
	return NULL;
}

Spike::~Spike()
{
}

void Spike::initSpike()
{
	spikeSprite = Sprite::createWithSpriteFrameName("spike" + std::to_string(RandomHelper::random_int(0, 2)) + ".png");
	spikeSprite->setFlippedX((RandomHelper::random_int(0, 1) == 1) ? true : false);
	spikeSprite->setAnchorPoint(Vec2(0.5, 0));
	addChild(spikeSprite);

	setupBoundary();
	BaseObject::initObject();
}

void Spike::setupBoundary()
{
	boundary.shape = SHAPE::rectangle;
	boundary.height = 150;
	boundary.width = 160;
	boundary.r = 80;
	boundary.active = true;
	boundary.offset = Vec2(-80, 0);
}
