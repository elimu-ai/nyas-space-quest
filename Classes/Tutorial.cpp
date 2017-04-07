#include "Tutorial.h"

Tutorial * Tutorial::create()
{
	Tutorial * tutorial = new Tutorial();

	if (tutorial && tutorial->init())
	{
		tutorial->autorelease();
		tutorial->initTutorial();
		return tutorial;
	}
	CC_SAFE_DELETE(tutorial);
	return NULL;
}

Tutorial::~Tutorial() {}

void Tutorial::initTutorial()
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("tutorial.plist");
	setupDirector();
	setupMovement();
	setupJump();
	BaseObject::initObject();
}

void Tutorial::setupDirector()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	winSize = Director::getInstance()->getWinSize(); //design size?
	frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	origin = Director::getInstance()->getVisibleOrigin(); // common to all maps i hope
}

void Tutorial::setupMovement()
{
	Sprite * hand = Sprite::createWithSpriteFrameName("hand.png");
	hand->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 6 - 40));
	Vec2 handPos = hand->getPosition();
	auto * handSeq = Sequence::create(MoveTo::create(1, handPos + Vec2(40, 0)), MoveTo::create(1, handPos - Vec2(40, 0)), NULL);
	auto * handAction = RepeatForever::create(handSeq);
	hand->runAction(handAction);

	Sprite * leftArrow = Sprite::createWithSpriteFrameName("arrow.png");
	leftArrow->setPosition(Vec2(visibleSize.width / 4 - 100, visibleSize.height / 6));
	leftArrow->setFlippedX(true);
	Sprite * rightArrow = Sprite::createWithSpriteFrameName("arrow.png");
	rightArrow->setPosition(Vec2(visibleSize.width / 4 + 100, visibleSize.height / 6));
	auto * arrowSeq = Sequence::create(ScaleTo::create(0.5, 1, 0.95), ScaleTo::create(0.5, 1, 1), NULL);
	auto * arrowAction = RepeatForever::create(arrowSeq);
	leftArrow->runAction(arrowAction);
	rightArrow->runAction(arrowAction->clone());

	this->addChild(hand);
	this->addChild(leftArrow);
	this->addChild(rightArrow);
	hand->setOpacity(200);
	leftArrow->setOpacity(200);
	rightArrow->setOpacity(200);
}

void Tutorial::setupJump()
{
	Sprite * doubleJump = Sprite::createWithSpriteFrameName("doubleJump.png");
	doubleJump->setPosition(Vec2(3 * visibleSize.width / 4, visibleSize.height / 2));
	this->addChild(doubleJump);
}


