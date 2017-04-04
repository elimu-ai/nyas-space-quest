//
//  GameScene.cpp
//  scrollerTest.x
//
//  Created by German Torres on 7/7/15.
//
//
#include "GameMapScene.h"
//#include "PuertoScene.h"

#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "spine/spine.h"
#include <string>
#include <sstream>
//#include <SDL.h>

USING_NS_CC;
using namespace CocosDenshion;


Scene* GameMap::createScene(std::string map)
{
    auto scene = Scene::create();
    auto layer = GameMap::create();

	//layer->loadMap();

    scene->addChild(layer);
    return scene;
}


bool GameMap::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	auto cache = SpriteFrameCache::getInstance();
	cache->removeUnusedSpriteFrames();
	cache->addSpriteFramesWithFile("common.plist");

	tipVector = Vector<Tip*>();
	coinVector = Vector<Coin*>();
	isCameraActive = true;

	setupDirector();
	setupGameplayNode();
	//setupButtonExit();
	setupLabels();
	setupParallax();
	setupKeyListener();

	//touch listener
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameMap::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameMap::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameMap::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
#endif
	gameplayNode->setScale(zoomFactor);
    this->scheduleUpdate();
    return true;
}

void GameMap::setupDirector()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	winSize = Director::getInstance()->getWinSize(); //design size?
	frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	origin = Director::getInstance()->getVisibleOrigin(); // common to all maps i hope
}

void GameMap::setupGameplayNode()
{
	gameplayNode = Node::create();
	this->addChild(gameplayNode);
}

void GameMap::setupTilemap(std::string map)
{
	tiledMap = TMXTiledMap::create(map);
	propLayer = tiledMap->getLayer("propertiesLayer");
#ifndef kTileDebug
	propLayer->setVisible(false);
#endif // !kTileDebug
	//auto planetLabel = Label::createWithTTF(tiledMap->getProperty("Name").asString(), "fonts/Akashi.ttf", 18);
	//addChild(planetLabel);
	//planetLabel->setPosition(300, 50);
	gameplayNode->addChild(tiledMap, -10);
}

void GameMap::setupButtonExit()
{
	auto buttonExit = ui::Button::create("botonSalir.png", "botonSalirSel.png", "botonSalir.png", ui::Widget::TextureResType::PLIST);
	buttonExit->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("sfx/button.wav");
			sceneExit();
			break;
		}
		default:
			break;
		}
	});
	buttonExit->setScale(0.45f);
	buttonExit->setOpacity(210);
	buttonExit->setAnchorPoint(Vec2(1, 1));
	buttonExit->setPosition(Vec2(origin.x - 20 + visibleSize.width, origin.y - 10 + visibleSize.height));
	this->addChild(buttonExit, 10);
}

void GameMap::setupLabels()
{
	//score
	scoreLabel = Label::createWithTTF("Stars: 0", "fonts/Andika-Regular.ttf", 24);
	scoreLabel->setAnchorPoint(Vec2(0, 0.5));
	scoreLabel->setPosition(Vec2(origin.x + 10,
		origin.y + visibleSize.height - 20));
	scoreLabel->enableGlow(Color4B::BLACK);
	this->addChild(scoreLabel);

	tipsLabel = Label::createWithTTF("#: 0", "fonts/Andika-Regular.ttf", 24);
	tipsLabel->setAnchorPoint(Vec2(0, 0.5));
	tipsLabel->setPosition(Vec2(origin.x + 10,
		origin.y + visibleSize.height - 40));
	tipsLabel->enableGlow(Color4B::BLACK);
	this->addChild(tipsLabel);
}

void GameMap::setupParallax()
{
	parallaxNodeBack = Node::create();
	parallaxNodeBack->setAnchorPoint(Vec2(0, 0));
	this->addChild(parallaxNodeBack, -5);

	parallaxNodeFront = Node::create();
	parallaxNodeFront->setAnchorPoint(Vec2(0, 0));
	this->addChild(parallaxNodeFront, 5);
}

void GameMap::setupKeyListener()
{
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameMap::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameMap::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, gameplayNode);
}

 void GameMap::loadMap()
{
    //joystick
    joystickSprite = Sprite::createWithSpriteFrameName("joystick.png");
    joystickSprite->setPosition(Vec2(-1000,-1000));
    this->addChild(joystickSprite);
    
    //load objects from tilemap
    auto objects = tiledMap->getObjectGroup("objects");
	
    //spawn point
    auto spawnPoint = objects->getObject("spawn");
    int x = spawnPoint["x"].asInt();
    int y = spawnPoint["y"].asInt();
    spawnMarker = ParticleGalaxy::create();
    spawnMarker->setPositionType(ParticleSystem::PositionType::GROUPED);
    spawnMarker->setScale(0.4f);
    spawnMarker->setPosition(Vec2(x,y));
    gameplayNode->addChild(spawnMarker);
    
    //end
    auto endPoint = objects->getObject("end");
    int endX = endPoint["x"].asInt();
    int endY = endPoint["y"].asInt();
	endObject = End::create();
	endObject->setPosition(Vec2(endX, endY));
	gameplayNode->addChild(endObject);
    
    //nave
    nave = Sprite::createWithSpriteFrameName("nave.png");
    gameplayNode->addChild(nave,2);
    nave->setAnchorPoint(Vec2(0.75,0));
    auto moveBy = MoveBy::create(2, Vec2(0, 7));
    auto moveBack = moveBy->reverse();
    auto seq1 = Sequence::create(moveBy, moveBack, nullptr);
    nave->runAction(RepeatForever::create(seq1));
    nave->setPosition(Vec2(x,y));

	//load platforms
	auto platforms = tiledMap->getObjectGroup("platforms")->getObjects();
	for (auto platform : platforms)
	{
		auto platformMap = platform.asValueMap();
		std::string platName = platformMap["name"].asString();
		int x = platformMap["x"].asInt();
		int y = platformMap["y"].asInt();
		auto sprite = Sprite::createWithSpriteFrameName(platName);
		if (RandomHelper::random_int(0, 1) == 1)
		{
			sprite->setFlippedX(true);
		}
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(x, y));
		gameplayNode->addChild(sprite);
	}
    
	//load tips
	auto tips = tiledMap->getObjectGroup("tips")->getObjects();
	if (!tips.empty())
	{
		for (auto tip : tips)
		{
			auto tipMap = tip.asValueMap();
			std::string tipName = tipMap["name"].asString();
			int x = tipMap["x"].asInt();
			int y = tipMap["y"].asInt();
			auto object = Tip::create(tipName);
			object->setAnchorPoint(Vec2::ZERO);
			object->setPosition(Vec2(x, y));
			tipVector.pushBack(object);
			gameplayNode->addChild(object);
			object->setTag(totalTips);
            totalTips++;
			auto textSprite = Sprite::createWithSpriteFrameName(tipName + "Text.png");
			textSprite->setAnchorPoint(Vec2(0.5, 1));
			textSprite->setPosition(Vec2(visibleSize.width / 2, 0.0f));
			addChild(textSprite, 200);
			tiptextVector.pushBack(textSprite);
		}
	}
    
    //load coins
    auto coins = tiledMap->getObjectGroup("coins")->getObjects();
    for (auto coin : coins)
    {
        auto coinMap = coin.asValueMap();
        int x = coinMap["x"].asInt();
        int y = coinMap["y"].asInt();
        auto object = Coin::create();
        object->setPosition(Vec2(x, y));
        coinVector.pushBack(object);
        gameplayNode->addChild(object);
        totalCoins++;
    }

	//load player
    player = Player::create(tiledMap);
    player->spawnPoint = Vec2(x,y);
    player->setPosition(player->spawnPoint);
    player->spawn();
    gameplayNode->addChild(player);
    
    //load bot
    gameplayNode->addChild(player->bot);
	player->bot->setPosition(player->spawnPoint + Vec2(0, 100));

    //load settings
    auto gravity = tiledMap->getProperty("Gravity");
    player->GRAVITY = gravity.asFloat();

	auto groundFriction = tiledMap->getProperty("GroundFriction");
	player->groundFriction = groundFriction.asFloat();
}

void GameMap::sceneExit()
{
	//SDL_Quit();
	/*
    auto menuScene = Puerto::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, menuScene));
	*/
}

void GameMap::setZoom(float z)
{
	zoomFactor = z;
	gameplayNode->setScale(z);
}

//Keyboard
#define LEFT_KEY cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW
#define RIGHT_KEY cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW
#define SPACE_KEY cocos2d::EventKeyboard::KeyCode::KEY_SPACE
void GameMap::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    keys[keyCode] = 0;
    switch (keyCode) {
        case RIGHT_KEY:
			player->accelerateHorizontal(true);
            horizontalKeysActive++;
            break;
        case LEFT_KEY:
			player->accelerateHorizontal(false);
            horizontalKeysActive++;
            break;
        case SPACE_KEY:
            actionKeyPressed();
            break;
            
        default:
            break;
    }
}

void GameMap::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    keys.erase(keyCode);
    switch (keyCode) {
        case RIGHT_KEY:
            horizontalKeysActive--;
			if (horizontalKeysActive <= 0)
			{
				player->horizontalAccel = 0;
				horizontalKeysActive = 0;
			}
            break;
        case LEFT_KEY:
            horizontalKeysActive--;
			if (horizontalKeysActive <= 0)
			{
				player->horizontalAccel = 0;
				horizontalKeysActive = 0;
			}
            break;
        default:
            break;
	}
}

//Touches
int joystickTouchId = -1;
Vec2 joystickTouchLocation;
bool GameMap::onTouchBegan(Touch* touch, cocos2d::Event *event)
{

	auto location = touch->getLocation();
	if (location.x < visibleSize.width / 2)
	{
		joystickTouchId = touch->getID();
		joystickTouchLocation = location;
		joystickSprite->setPosition(location);
	}
	else
	{
		actionKeyPressed();
	}
	return true;
}

void GameMap::onTouchMoved(Touch *touch, Event *event)
{
	auto location = touch->getLocation();
	if (joystickTouchId == touch->getID())
	{
		if (location.x > joystickTouchLocation.x && joystickActive != 1)
		{
			player->accelerateHorizontal(true);
            joystickActive = 1;
		}
		else if (location.x < joystickTouchLocation.x && joystickActive != -1)
		{
			player->accelerateHorizontal(false);
            joystickActive = -1;
		}
	}
}

void GameMap::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (joystickTouchId == touch->getID())
	{
        joystickActive = 0;
		player->horizontalAccel = 0;
		joystickTouchId = -1;
		joystickSprite->setPosition(Vec2(-1000, -1000));
	}
}

void GameMap::update(float dt)
{
	auto audio = SimpleAudioEngine::getInstance();
	if (pauseTimer > 0)
	{
		pauseTimer -= dt;
        if (pauseTimer < 3) player->update(dt, tiledMap);
	}
	else
	{
        audio->resumeBackgroundMusic();
		player->update(dt, tiledMap);
	}
	if (isCameraActive)
	{
		this->centerCamera(player->getPosition());
	}
	this->parallaxMove();
	//tips
	for (Tip * tip : tipVector)
	{
		auto tipTextSprite = tiptextVector.at(tip->getTag());

		if (player->checkIntersect(tip))
		{
			spawnMarker->setPosition(tip->getPosition());
			player->spawnPoint = spawnMarker->getPosition();

			char scoreString[20];
			if (!tip->consumed)
			{
				tip->consumed = true;
				grabbedTips++;
				//play audio here?
				auto audio = SimpleAudioEngine::getInstance();
				audio->pauseBackgroundMusic();
				tip->playAudio();
				player->pausePlayer();
				pauseTimer = 0;
			}
			sprintf(scoreString, "#: %i", grabbedTips);
			tipsLabel->setString(scoreString); 
			if (tipTextSprite->getTag() != -1) //is popping up
			{
				auto popup = MoveTo::create(0.2f, Vec2(tipTextSprite->getPositionX(), 125.0f));
				tipTextSprite->stopAllActions();
				tipTextSprite->runAction(popup);
				tipTextSprite->setTag(-1);
			}
			tip->update(true);
		}
		else
		{
			if (tipTextSprite->getTag() != 1) //is popping down
			{
				auto popdn = MoveTo::create(0.5f, Vec2(tipTextSprite->getPositionX(), 0.0f));
				tipTextSprite->stopAllActions();
				tipTextSprite->runAction(popdn);
				tipTextSprite->setTag(1);
			}
			tip->update(false);
		}
	}
	//coins
	for (Coin * coin : coinVector)
	{
		if (player->checkIntersect(coin))
		{
			char scoreString[20];
			if (!coin->consumed)
			{
				coin->consume();
				grabbedCoins++;
				
			}
			sprintf(scoreString, "Stars: %i", grabbedCoins);
			scoreLabel->setString(scoreString);
		}
	}
	if (player->checkIntersect(endObject))
	{
		writeData();
		//auto loadScene = Loading::createScene(kPuerto);
		//Director::getInstance()->replaceScene(TransitionFade::create(0.3f, loadScene));
	}
}

void GameMap::parallaxMove()
{
    float scale = Director::getInstance()->getContentScaleFactor();
	float p = 0.2f;
	//get the amount moved by gameplayNode
	float dx = gameplayNode->getPositionX() - previousGPNx;
	float dy = gameplayNode->getPositionY() - previousGPNy;
	previousGPNx = gameplayNode->getPositionX();
	previousGPNy = gameplayNode->getPositionY();
	auto children = parallaxNodeBack->getChildren();
	float xMovement = p*dx;
	for (auto child : children)
	{
		child->setPositionX(child->getPositionX() + xMovement);//round(p*dx*scale)/scale);
	}

	p = 1.1;
	auto children_b = parallaxNodeFront->getChildren();
	for (auto child : children_b)
	{
		float x = parallaxNodeFront->convertToWorldSpace(child->getPosition()).x;
		float width = child->getContentSize().width;
		if (dx > 0) // <==
		{
			if (x > visibleSize.width + 50)
			{
				child->setPositionX(-visibleSize.width - RandomHelper::random_int(10, (int)visibleSize.width));
			}
			else
			{
				child->setPositionX(child->getPositionX() + p*dx);
			}

		}
		else if (dx < 0)  // ==>
		{
			if (x < -width - 50)
			{
				child->setPositionX(visibleSize.width + RandomHelper::random_int(10, (int)visibleSize.width));
			}
			else
			{
				child->setPositionX(child->getPositionX() + p*dx);
			}
		}
	}
	if (verticalParallaxEnabled)
	{
		parallaxNodeBack->setPositionY(parallaxNodeBack->getPositionY() + 0.2*dy);
		parallaxNodeFront->setPositionY(parallaxNodeFront->getPositionY() + 1.1*dy);
	}
}

void GameMap::writeData()
{
	auto ud = UserDefault::getInstance();
	if (ud->getIntegerForKey("levelUnlock", 0) <= levelId)
	{
		ud->setIntegerForKey("levelUnlock", levelId + 1);
	}
	std::string starsString = "stars" + std::to_string(levelId);
	std::string tipsString = "tips" + std::to_string(levelId);
	if (ud->getFloatForKey(starsString.c_str(), 0.0f) < grabbedCoins / totalCoins * 100)
	{
		ud->setFloatForKey(starsString.c_str(), grabbedCoins / totalCoins * 100);
	}
	if (ud->getFloatForKey(tipsString.c_str(), 0.0f) < grabbedTips / totalTips * 100)
	{
		ud->setFloatForKey(tipsString.c_str(), grabbedTips / totalTips * 100);
	}
	ud->flush();
}

void GameMap::showTotal()
{
	auto ud = UserDefault::getInstance();
	float totalStars = ud->getFloatForKey("stars0", 0.0f) + ud->getFloatForKey("stars1", 0.0f) + ud->getFloatForKey("stars2", 0.0f) + ud->getFloatForKey("stars3", 0.0f);
	float totalTips = ud->getFloatForKey("tips0", 0.0f) + ud->getFloatForKey("tips1", 0.0f) + ud->getFloatForKey("tips2", 0.0f) + ud->getFloatForKey("tips3", 0.0f);
	char scoreString[30];
	sprintf(scoreString, "Stars: 0");
	scoreLabel->setString(scoreString);
	sprintf(scoreString, "#:0");
	tipsLabel->setString(scoreString);
}

void GameMap::hurtPlayer()
{
	if (!player->isHurt)
	{
		player->gotHurt();
	}
}

void GameMap::actionKeyPressed()
{
	player->jump();
}

void GameMap::centerCamera(Vec2 position)
{
    //TODO: make this smoother
    float scale = Director::getInstance()->getContentScaleFactor();
    float x = MAX(position.x*zoomFactor, winSize.width/2);
    float y = MAX((position.y+50)*zoomFactor, winSize.height/2);
    x = MIN(x, zoomFactor*(tiledMap->getMapSize().width * tiledMap->getTileSize().width/scale) - winSize.width/2);
    y = MIN(y, zoomFactor*(tiledMap->getMapSize().height * tiledMap->getTileSize().height/scale) - winSize.height/2);
    Vec2 actualPosition = Vec2(x,y);
    
    Vec2 centerOfView = Vec2(winSize.width/2, winSize.height/2);
    Vec2 viewPoint = centerOfView - actualPosition;

    float newX = round(viewPoint.x * scale) / scale;
    float newY = round(viewPoint.y * scale) / scale;
    
    gameplayNode->setPosition(Vec2(newX, newY));
}

void GameMap::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
