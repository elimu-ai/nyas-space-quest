#include "Planet.h"



Planet * Planet::create(int n)
{
	Planet * planet = new Planet();

	if (planet && planet->init())
	{
		planet->autorelease();
		planet->initPlanet(n);
		return planet;
	}
	CC_SAFE_DELETE(planet);
	return NULL;
}


Planet::~Planet()
{
}

void Planet::initPlanet(int n)
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("planets.plist");
	int planetNumber = n;
	if (n == -1)
	{
		planetNumber = RandomHelper::random_int(0, 3);
	}
	auto planetSprite = Sprite::createWithSpriteFrameName(std::to_string(planetNumber) + ".png");
	auto glowSprite = Sprite::createWithSpriteFrameName("glow.png");
	
	auto scaleUp = ScaleTo::create(1, 1.03);
	auto scaleDown = ScaleTo::create(RandomHelper::random_real(0.5, 1.5), 1);
	auto seqScale = Sequence::create(scaleUp, scaleDown, NULL);
	auto repScale = RepeatForever::create(seqScale);
	glowSprite->runAction(repScale);

	auto rotate = RotateBy::create(2.5, RandomHelper::random_int(-12,12));
	auto repRotate = RepeatForever::create(rotate);
	planetSprite->runAction(repRotate);
	
	this->addChild(glowSprite);
	this->addChild(planetSprite);
}
