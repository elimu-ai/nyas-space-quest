//
//  MenuScene.cpp
//  scrollerTest.x
//
//  Created by German Torres on 7/22/15.
//
//

#include "MenugScene.h"
//#include "LoadingScene.h"
//#include "BitacoraScene.h"
//#include "OptionsScene.h"
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
	setupLabel();
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
    winSize = director->getWinSize(); //design size?
    frameSize = director->getOpenGLView()->getFrameSize();
    origin = director->getVisibleOrigin();
}

void Menug::loadAudio()
{
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadEffect("sfx/button.wav");
}

void Menug::cacheImages()
{
    auto cache = SpriteFrameCache::getInstance();
    //cache->removeSpriteFrames();
    cache->addSpriteFramesWithFile("menu.plist");
	cache->addSpriteFramesWithFile("common.plist");
}

void Menug::setupLogo()
{
	auto stars = Sprite::createWithSpriteFrameName("stars.jpg");
	stars->setAnchorPoint(Vec2::ZERO);
	this->addChild(stars);
    auto logo = Sprite::createWithSpriteFrameName("logo_akua.png");
    //logo->setScale(0.25);
    logo->setAnchorPoint(Vec2(0, 1));
    logo->setPosition(Vec2(10, visibleSize.height - 0));
    this->addChild(logo);
}

void Menug::setupNave()
{
    auto nave = Sprite::createWithSpriteFrameName("naveMenu.png");
    nave->setPosition(Vec2(origin.x + visibleSize.width / 2 - 10, origin.y + visibleSize.height / 2 - 30));
    //nave->setScale(0.4);
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
    buttonPlay->setPosition(Vec2(buttonX, 380));
    
    auto buttonBitacora = ui::Button::create("botonBitacora.png", "botonBitacoraSel.png", "botonBitacora.png", ui::Widget::TextureResType::PLIST);
    buttonBitacora->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                //TODO: Bitacora boton
                auto audio = SimpleAudioEngine::getInstance();
                audio->playEffect("sfx/button.wav");
                Menug::loadBitacora();
                break;
            }
            default:
                break;
        }
    });
    buttonBitacora->setScale(0.9f);
    buttonBitacora->setPosition(Vec2(buttonX, 250));
    
    auto buttonOptions = ui::Button::create("botonOpciones.png", "botonOpcionesSel.png", "botonOpciones.png", ui::Widget::TextureResType::PLIST);
    buttonOptions->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                //TODO: options boton
                auto audio = SimpleAudioEngine::getInstance();
                audio->playEffect("sfx/button.wav");
				Menug::loadOptions();
                
                break;
            }
            default:
                break;
        }
    });
    buttonOptions->setScale(0.9f);
    buttonOptions->setPosition(Vec2(buttonX, 120));
    
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
    this->addChild(buttonBitacora);
    this->addChild(buttonOptions);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    this->addChild(buttonExit);
#endif
}

void Menug::setupLabel()
{
    Label* label1 = Label::createWithTTF("Version 0.1", "fonts/Andika-Regular.ttf", 20);
    label1->setPosition(Vec2(visibleSize.width - label1->getContentSize().width / 2 - 20, 20));
    label1->setColor(Color3B::WHITE);
    this->addChild(label1);
}

void Menug::loadGame()
{
    //auto loadScene = Loading::createScene(kPuerto);
    //Director::getInstance()->replaceScene(TransitionFade::create(0.3f, loadScene));
}

void Menug::loadBitacora()
{
	//auto bitacoraScene = Loading::createScene(kBitacora);
	//Director::getInstance()->replaceScene(TransitionFade::create(0.5f, bitacoraScene));
}

void Menug::loadOptions()
{
	//auto optionsScene = Options::createScene();
	//Director::getInstance()->replaceScene(TransitionFade::create(1.0f, optionsScene));
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



