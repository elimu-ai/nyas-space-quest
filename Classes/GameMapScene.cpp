//
//  GameScene.cpp
//  
//
//  Created by German Torres on 7/7/15.
//
//
#include "GameMapScene.h"
#include "MenugScene.h"

#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "spine/spine.h"
#include <string>
#include <sstream>
#include "LanguageManager.h"

USING_NS_CC;
using namespace CocosDenshion;


Scene* GameMap::createScene(std::string map)
{
	auto scene = Scene::create();
	auto layer = GameMap::create();
	scene->addChild(layer);
	return scene;
}


bool GameMap::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto cache = SpriteFrameCache::getInstance();
	cache->removeUnusedSpriteFrames();
	cache->addSpriteFramesWithFile("common.plist");

	numberDisplayVector = Vector<NumberDisplay*>();
	coinVector = Vector<Coin*>();
	isCameraActive = true;

	setupDirector();
	setupGameplayNode();
	setupLabels();
	setupParallax();
	setupKeyListener();
	setupNumberDisplayBG();
	setupTutorial();
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
#endif 
	gameplayNode->addChild(tiledMap, -10);
}

void GameMap::setupLabels()
{
	//score
	scoreLabel = Label::createWithTTF(LanguageManager::getString("stars") + ": 0", LanguageManager::getString("font"), 24);
	scoreLabel->setAnchorPoint(Vec2(0, 0.5));
	scoreLabel->setPosition(Vec2(origin.x + 10,
		origin.y + visibleSize.height - 20));
	scoreLabel->enableGlow(Color4B::BLACK);
	this->addChild(scoreLabel);
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

void GameMap::setupNumberDisplayBG()
{
	numberDisplayBG = Sprite::createWithSpriteFrameName("stars.jpg");
	numberDisplayBG->setAnchorPoint(Vec2(0, 0));
	numberDisplayBG->setOpacity(230);
	numberDisplayBG->setPosition(0, visibleSize.height);

	this->addChild(numberDisplayBG);
}

void GameMap::setupTutorial()
{
	tutorial = Tutorial::create();
	this->addChild(tutorial);
}

void GameMap::loadMap()
{
	//joystick
	joystickSprite = Sprite::createWithSpriteFrameName("joystick.png");
	joystickSprite->setPosition(Vec2(-1000, -1000));
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
	spawnMarker->setPosition(Vec2(x, y));
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
	gameplayNode->addChild(nave, 2);
	nave->setAnchorPoint(Vec2(0.75, 0));
	auto moveBy = MoveBy::create(2, Vec2(0, 7));
	auto moveBack = moveBy->reverse();
	auto seq1 = Sequence::create(moveBy, moveBack, nullptr);
	nave->runAction(RepeatForever::create(seq1));
	nave->setPosition(Vec2(x, y));

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

	//load Number Displays
	auto numberDisplays = tiledMap->getObjectGroup("tips")->getObjects();
	if (!numberDisplays.empty())
	{
		for (auto tip : numberDisplays)
		{
			auto tipMap = tip.asValueMap();
			int x = tipMap["x"].asInt();
			int y = tipMap["y"].asInt();
			auto object = NumberDisplay::create(RandomHelper::random_int(1,10), numberDisplayBG);
			object->setAnchorPoint(Vec2::ZERO);
			object->setPosition(Vec2(x, y));
			numberDisplayVector.pushBack(object);
			gameplayNode->addChild(object);
			object->setTag(totalTips);
			totalTips++;
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
	player->spawnPoint = Vec2(x, y);
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
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
	{
		Scene * scene = Menug::createScene();

		Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5, scene));
	}

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
	if (!playerPaused)
	{
		player->update(dt, tiledMap);
	}

	if (isCameraActive)
	{
		this->centerCamera(player->getPosition());
	}
	this->parallaxMove();

	//numberDisplays
	for (NumberDisplay * numberDisplay : numberDisplayVector)
	{
		if (player->checkIntersect(numberDisplay))
		{
			spawnMarker->setPosition(numberDisplay->getPosition());
			player->spawnPoint = spawnMarker->getPosition();

			if (!numberDisplay->consumed)
			{
				player->pausePlayer();
				playerPaused = true;
			}
			else
			{
				playerPaused = false;
			}

			numberDisplay->update(true);
		}
		else
		{
			numberDisplay->update(false);
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
			sprintf(scoreString, "%s: %i", LanguageManager::getString("stars").c_str(), grabbedCoins);
			scoreLabel->setString(scoreString);
		}
	}
	//Show tutorial only a the beginning of the map
	if (gameplayNode->getPosition().x < -50)
	{
		tutorial->setVisible(false);
	}
	else
	{
		tutorial->setVisible(true);
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
	std::string tipsString = "numberDisplays" + std::to_string(levelId);
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
	float x = MAX(position.x*zoomFactor, winSize.width / 2);
	float y = MAX((position.y + 50)*zoomFactor, winSize.height / 2);
	x = MIN(x, zoomFactor*(tiledMap->getMapSize().width * tiledMap->getTileSize().width / scale) - winSize.width / 2);
	y = MIN(y, zoomFactor*(tiledMap->getMapSize().height * tiledMap->getTileSize().height / scale) - winSize.height / 2);
	Vec2 actualPosition = Vec2(x, y);

	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
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
