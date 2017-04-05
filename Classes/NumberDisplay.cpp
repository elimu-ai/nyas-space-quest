#include "NumberDisplay.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

NumberDisplay * NumberDisplay::create(std::string tipName, Sprite * _bg)
{
	NumberDisplay * tip = new NumberDisplay();

	if (tip && tip->init())
	{
		tip->autorelease();
		tip->initNumberDisplay(tipName, _bg);
		return tip;
	}
	CC_SAFE_DELETE(tip);
	return NULL;
}

NumberDisplay::~NumberDisplay()
{
	popDownSeq->release();
	popUpSeq->release();
}

void NumberDisplay::initNumberDisplay(std::string tipName, Sprite * _bg)
{
	name = tipName;
	setupBoundary();
	setupMessageSprite(tipName);
	setupSprite();
	setupAudio(tipName);
	consumed = false;
	this->setScale(0.9);
	bg = _bg;
	BaseObject::initObject();
}

void NumberDisplay::setupBoundary()
{
	boundary.shape = SHAPE::circle;
	boundary.active = true;
	boundary.r = 20;
}

void NumberDisplay::setupMessageSprite(std::string tipName)
{
	popUpSeq = Sequence::create(ScaleTo::create(0.3, 1), nullptr);
	popUpSeq->retain();
	popDownSeq = Sequence::create(ScaleTo::create(0.5, 0.0f), nullptr);
	popDownSeq->retain();
	auto tipSprite = Sprite::createWithSpriteFrameName(tipName + ".png");
	tipSprite->setTag(55);
	messageSprite = Sprite::createWithSpriteFrameName("tipBg.png");
	tipSprite->setPosition(Vec2(messageSprite->getContentSize().width / 2, messageSprite->getContentSize().height / 2));
	messageSprite->addChild(tipSprite);
	messageSprite->setScale(0.0f);
	messageSprite->setAnchorPoint(Vec2(0.5, 0));
	isMessagevisible = false;
	this->addChild(messageSprite);
}

void NumberDisplay::setupSprite()
{
	auto sprite = Sprite::createWithSpriteFrameName("tip.png");
	auto moveUp = EaseInOut::create(MoveBy::create(2, Vec2(0, 5.0f)), 2);
	auto moveBack = EaseInOut::create(MoveBy::create(2, Vec2(0, -5.0f)), 2);
	auto seq1 = Sequence::create(moveUp, moveBack, nullptr);
	sprite->runAction(RepeatForever::create(seq1));
	this->addChild(sprite);
}

void NumberDisplay::setupAudio(std::string tipName)
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/bot.wav");
	audio->preloadEffect(("sfx/tips/" + name + ".mp3").c_str());
}

void NumberDisplay::update(bool hit)
{
	if (hit && !isMessagevisible)
	{
		auto audio = SimpleAudioEngine::getInstance();
		//set this var to true for the name of the Tip, save it to file
		auto ud = UserDefault::getInstance();
		ud->setBoolForKey(name.c_str(), true);
		ud->flush();
		isMessagevisible = true;
		audio->playEffect("sfx/bot.wav");
		messageSprite->stopAllActions();
		messageSprite->runAction(popUpSeq);
		bg->setVisible(true);
	}
	else if (!hit && isMessagevisible)
	{
		isMessagevisible = false;
		messageSprite->stopAllActions();
		messageSprite->runAction(popDownSeq);
		bg->setVisible(false);
	}
}

void NumberDisplay::playAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect(("sfx/tips/" + name + ".mp3").c_str());
}
