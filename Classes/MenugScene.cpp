//
//  MenuScene.cpp
//  scrollerTest.x
//
//  Created by German Torres on 7/22/15.
//
//

#include "MenugScene.h"
#include "LoadingScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

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
	if (CCApplication::getInstance()->getCurrentLanguage() == LanguageType::SWAHILI)
	{
		CCLOG("Swahili language device found");
	}
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
	if (!audio->isBackgroundMusicPlaying())
	{
		audio->stopBackgroundMusic();
		audio->playBackgroundMusic("sfx/marsMusic.mp3", true);
	}
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
    //TODO: extract a lot of this. still doing too much here
    float buttonX = visibleSize.width - 120;
    
    auto buttonPlay = ui::Button::create("botonJugar.png", "botonJugarSel.png", "botonJugar.png", ui::Widget::TextureResType::PLIST);
    buttonPlay->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                auto audio = SimpleAudioEngine::getInstance();
                audio->playEffect("sfx/button.wav");
                Menug::loadGame();
                break;
            }
            default:
                break;
        }
    });
    buttonPlay->setScale(0.9f);
    buttonPlay->setPosition(Vec2(origin.x + 3* visibleSize.width / 4, origin.y + visibleSize.height / 2 - 50));
    
   
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    auto buttonExit = ui::Button::create("botonSalir.png", "botonSalirSel.png", "botonSalir.png", ui::Widget::TextureResType::PLIST);
    buttonExit->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                auto audio = SimpleAudioEngine::getInstance();
                audio->playEffect("sfx/button.wav");
                Menug::exitAkua();
                break;
            }
            default:
                break;
        }
    });
    buttonExit->setScale(0.4f);
    buttonExit->setPosition(Vec2(origin.x + 60, origin.y + 40));
#endif	
    this->addChild(buttonPlay);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    //this->addChild(buttonExit);
#endif
}

void Menug::setupVersionLabel()
{
    Label* label1 = Label::createWithTTF("Version 0.1", "fonts/AndikaLowerCase-Regular_5dp.ttf", 20);
    label1->setPosition(Vec2(visibleSize.width - label1->getContentSize().width / 2 - 20, 20));
    label1->setColor(Color3B::WHITE);
    this->addChild(label1);
}

void Menug::loadGame()
{
    auto loadScene = Loading::createScene(kNumberIdA);
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



