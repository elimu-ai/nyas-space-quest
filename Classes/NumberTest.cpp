#include "NumberTest.h"
#include "SimpleAudioEngine.h"
#include "LanguageManager.h"
#include "Planet.h"


USING_NS_CC;
using namespace CocosDenshion;

NumberTest * NumberTest::create(int _number, Sprite * _bg)
{
	NumberTest * tip = new NumberTest();

	if (tip && tip->init())
	{
		tip->autorelease();
		tip->initNumberTest(_number, _bg);
		return tip;
	}
	CC_SAFE_DELETE(tip);
	return NULL;
}

NumberTest::~NumberTest()
{

}

void NumberTest::initNumberTest(int _number, Sprite * _bg)
{
	number = _number;
	setupDirector();
	setupBoundary();
	setupSprite();
	setupAudio();
	setupLabel();
	consumed = false;
	this->setScale(0.9);
	bg = _bg;
	BaseObject::initObject();
}

void NumberTest::setupDirector()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	winSize = Director::getInstance()->getWinSize(); //design size?
	frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	origin = Director::getInstance()->getVisibleOrigin(); // common to all maps i hope
}

void NumberTest::setupBoundary()
{
	boundary.shape = SHAPE::circle;
	boundary.active = true;
	boundary.r = 20;
}

void NumberTest::setupSprite()
{
	auto sprite = Sprite::createWithSpriteFrameName("tip.png");
	auto moveUp = EaseInOut::create(MoveBy::create(2, Vec2(0, 5.0f)), 2);
	auto moveBack = EaseInOut::create(MoveBy::create(2, Vec2(0, -5.0f)), 2);
	auto seq1 = Sequence::create(moveUp, moveBack, nullptr);
	sprite->runAction(RepeatForever::create(seq1));
	this->addChild(sprite);
}

void NumberTest::setupAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/bot.wav");
	langCode = "en";
	if (CCApplication::getInstance()->getCurrentLanguage() == LanguageType::SWAHILI)
		langCode = "sw";
	for (int i = 0; i <= 10; i++)
	{
		audio->preloadEffect(("sfx/" + langCode + "/digit_" + std::to_string(i) + ".wav").c_str());
	}
}

void NumberTest::setupLabel()
{
	numberLabel = Label::createWithTTF(std::to_string(number), LanguageManager::getString("font"), 50);
	numberLabel->setPosition(Vec2(0, 100));
	auto action0 = ScaleTo::create(0.3f, 1.1f, 1.1f);
	auto action1 = ScaleTo::create(0.3f, 0.99f, 0.99f);
	ActionInterval *bouncingAction = Sequence::create(action0, action1, nullptr);
	auto action = RepeatForever::create(bouncingAction);
	numberLabel->runAction(action);
	numberLabel->setVisible(false);
	this->addChild(numberLabel);
}

void NumberTest::update(bool hit)
{
	if (hit && !isMessagevisible && !consumed)
	{
		auto audio = SimpleAudioEngine::getInstance();
		isMessagevisible = true;
		audio->playEffect("sfx/bot.wav");

		Vector<FiniteTimeAction *>  allActions;
		auto action = MoveTo::create(1.5, Vec2(0, 0));
		TargetedAction * t1 = TargetedAction::create(bg, action);
		allActions.pushBack(t1);

		Label * label = Label::createWithTTF("", LanguageManager::getString("font"), 150);
		bg->addChild(label);
		label->setPosition(Vec2(visibleSize.width - 125, visibleSize.height / 2));
		label->setScale(0.9);

		auto scaleTo1 = ScaleTo::create(1.3, 1);
		int vLevel = 2;
		int hLevel = 1;
		for (int i = 1; i <= number; i++)
		{
			auto planet = Planet::create(-1);
			planet->setPosition(Vec2(105 * hLevel - 40, vLevel * visibleSize.height / 3));
			bg->addChild(planet);
			hLevel++;
			if (i == 5)
			{
				vLevel = 1;
				hLevel = 1;
			}
			planet->setScale(0.01);
			TargetedAction * tA = TargetedAction::create(planet, scaleTo1);
			allActions.pushBack(tA);
			auto playAudio = CallFunc::create([this, i, label]() {

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect(("sfx/" + langCode + "/digit_" + std::to_string(i) + ".wav").c_str());
				label->setString(std::to_string(i));
			});
			allActions.pushBack(playAudio);
		}
		auto * scaleLabel = TargetedAction::create(label, scaleTo1);
		allActions.pushBack(scaleLabel);
		auto * delay = DelayTime::create(1.5);
		allActions.pushBack(delay);
		auto * moveUp = MoveTo::create(1.5, Vec2(0, visibleSize.height));
		allActions.pushBack(moveUp);
		auto consumeNumberDisplay = CallFunc::create([this]() {
			consumed = true;
		});
		allActions.pushBack(consumeNumberDisplay);
		auto seq = Sequence::create(allActions);
		bg->stopAllActions();
		bg->runAction(seq);
		numberLabel->setVisible(true);
	}
	else if (!hit && isMessagevisible)
	{
		isMessagevisible = false;
		auto action = MoveTo::create(1.5, Vec2(0, visibleSize.height));
		bg->stopAllActions();
		bg->runAction(action);
		bg->removeAllChildren();
	}

	if (hit && !isMessagevisible && consumed)
	{
		isMessagevisible = true;
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect(("sfx/" + langCode + "/digit_" + std::to_string(number) + ".wav").c_str());
	}
}

