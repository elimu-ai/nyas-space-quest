//
//  MenuScene.cpp
//  
//
//  Created by German Torres on 7/22/15.
//
//

#include "MenugScene.h"
#include "LoadingScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "LanguageManager.h"
#include "Planet.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene * Menug::createScene()
{
	auto scene = Scene::create();
	auto layer = Menug::create();
	scene->addChild(layer);
	return scene;
}

bool Menug::init()
{
	if (!LayerColor::initWithColor(Color4B::BLACK))
	{
		return false;
	}

	setupDirector();
	loadAudio();
	cacheImages();
	setupLogo();
	setupNave();
	setupButtons();
	setupVersionLabel();
	this->setKeypadEnabled(true);
	auto lm = LanguageManager::getInstance();
	return true;
}

void Menug::setupDirector()
{
	auto director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	winSize = director->getWinSize();
	frameSize = director->getOpenGLView()->getFrameSize();
	origin = director->getVisibleOrigin();
}

void Menug::loadAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/button.wav");
	audio->stopBackgroundMusic();
	audio->playBackgroundMusic("sfx/introMusic.mp3", true);
}

void Menug::cacheImages()
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("menu.plist");
	cache->addSpriteFramesWithFile("common.plist");
}

void Menug::setupLogo()
{
	auto stars = Sprite::createWithSpriteFrameName("stars.jpg");
	stars->setAnchorPoint(Vec2::ZERO);
	this->addChild(stars);

	auto logo = Sprite::createWithSpriteFrameName("nyaLogo.png");
	logo->setScale(0.75);
	logo->setAnchorPoint(Vec2(0.5, 1));
	logo->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height + 100));
	this->addChild(logo);
	auto fadeOut = FadeTo::create(13, 120);
	auto fadeIn = FadeIn::create(5);
	auto seq = Sequence::create(fadeOut, fadeIn, nullptr);
	logo->runAction(RepeatForever::create(seq));
}

void Menug::setupNave()
{
	auto nave = Sprite::createWithSpriteFrameName("naveMenu.png");
	nave->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2 - 50));
	auto moveBy = MoveBy::create(2, Vec2(0, 7));
	auto moveBack = moveBy->reverse();
	auto seq1 = Sequence::create(moveBy, moveBack, nullptr);
	nave->runAction(RepeatForever::create(seq1));
	this->addChild(nave);
}

void Menug::setupButtons()
{
	auto buttonPlayA = ui::Button::create("buttonPlay.png", "buttonPlaySel.png", "buttonPlay.png", ui::Widget::TextureResType::PLIST);
	buttonPlayA->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			auto audio = SimpleAudioEngine::getInstance();
			audio->playEffect("sfx/button.wav");
			Menug::loadGame(kNumberIdA);
			break;
		}
		default:
			break;
		}
	});
	buttonPlayA->setScale(0.9f);
	buttonPlayA->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4 - 30, origin.y + visibleSize.height / 2));

	Planet * planetA = Planet::create(1);
	planetA->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4 + 60, origin.y + visibleSize.height / 2));

	auto buttonPlayB = ui::Button::create("buttonPlay.png", "buttonPlaySel.png", "buttonPlay.png", ui::Widget::TextureResType::PLIST);
	buttonPlayB->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			auto audio = SimpleAudioEngine::getInstance();
			audio->playEffect("sfx/button.wav");
			Menug::loadGame(kNumberIdB);
			break;
		}
		default:
			break;
		}
	});
	buttonPlayB->setScale(0.9f);
	buttonPlayB->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4 - 30, origin.y + visibleSize.height / 2 - 120));

	Planet * planetB = Planet::create(3);
	planetB->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4 + 60, origin.y + visibleSize.height / 2 - 120));
	planetB->setColor(Color3B::GRAY);

	auto ud = UserDefault::getInstance();
	if (ud->getIntegerForKey("levelUnlock", 0) != 10)
	{
		buttonPlayB->setEnabled(false);
		buttonPlayB->setOpacity(100);
	}
	else
	{
		this->addChild(planetB);
	}

	this->addChild(planetA);
	this->addChild(buttonPlayA);
	this->addChild(buttonPlayB);
}

void Menug::setupVersionLabel()
{
	Label* label1 = Label::createWithTTF(LanguageManager::getString("language").c_str(), LanguageManager::getString("font").c_str(), 10);
	label1->setPosition(Vec2(visibleSize.width - label1->getContentSize().width / 2 - 20, 20));
	label1->setColor(Color3B::WHITE);
	this->addChild(label1);
}

void Menug::loadGame(int level)
{
	auto loadScene = Loading::createScene(level);
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, loadScene));
}

void Menug::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (keycode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
		Director::getInstance()->end();
}

void Menug::exitAkua()
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}



