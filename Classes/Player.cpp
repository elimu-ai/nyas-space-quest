//
//  PlayerNode.cpp
//  scrollerTest.x
//
//  Created by German Torres on 7/7/15.
//
//

#include "Player.h"

#include "spine/spine.h"
#include "SimpleAudioEngine.h"



USING_NS_CC;
using namespace CocosDenshion;

Player * Player::create(TMXTiledMap * tmap)
{
	Player * player = new Player();

	if (player && player->init())
	{
		player->autorelease();
		player->initPlayer(tmap);
		return player;
	}
	CC_SAFE_DELETE(player);
	return NULL;
}

Player::~Player() {}

void Player::initPlayer(TMXTiledMap * tmap)
{
	setupAudio();

	setupSkNode();
	setupBoundary();

	auto ud = UserDefault::getInstance();
	if (ud->getIntegerForKey("levelUnlock", 1) >= 0)
	{
		setupJetParticle();
		jumpsEnabled = 2;
	}

	phyState = PHYSICS_STATE::air;
	animState = ANIMATION_STATE::idle;
	this->setAnchorPoint(Vec2(0.5, 0));

	tiledMap = tmap;
	propLayer = tiledMap->getLayer("propertiesLayer");

	float scale = Director::getInstance()->getContentScaleFactor();
	scaledTileHeight = tiledMap->getTileSize().height / scale;
	scaledTileWidth = tiledMap->getTileSize().width / scale;

	bot = Bot::create();

	BaseObject::initObject();
	isHurt = false;
	jumpCounter = jumpsEnabled;
}

void Player::setupAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/jump.wav");
	audio->preloadEffect("sfx/hurt.wav");
}

void Player::setupJetParticle()
{
	jp = ParticleMeteor::create();
	jp->setScale(0.5f);
	jp->setGravity(Vec2::ZERO);
	jp->setPosition(-22, 50);
	jp->setLife(0.3f);
	jp->setSpeed(190);
	jp->setSpeedVar(10);
	jp->setAngle(-135);
	jp->setAngleVar(14);
	jp->setEndSize(16);
	jp->setEndSizeVar(16);
	jp->setStartColor(Color4F::ORANGE);
	jp->setEndColor(Color4F::BLACK);
	jp->setEndColorVar(Color4F::BLACK);
	jp->setDuration(0.5f);
	jp->setPositionType(ParticleSystem::PositionType::FREE);

	addChild(jp);
}

void Player::setupSkNode()
{
	skNode = spine::SkeletonAnimation::createWithJsonFile("spine/Nya.json", "spine/Nya.atlas", 0.09f);
	skNode->setAnimation(0, "idle", true);

	skNode->setMix("walk", "run", 0.2f);
	skNode->setMix("walk", "idle", 0.2f);
	skNode->setMix("walk", "jump_up", 0.05f);

	skNode->setMix("idle", "walk", 0.1f);
	skNode->setMix("idle", "jump_up", 0.05f);

	skNode->setMix("run", "walk", 0.2f);
	skNode->setMix("run", "idle", 0.2f);
	skNode->setMix("run", "jump_up", 0.05f);

	skNode->setMix("jump_up", "jump_down", 0.2f);

	skNode->setMix("jump_down", "idle", 0.2f);
	skNode->setMix("jump_down", "walk", 0.1f);
	skNode->setMix("jump_down", "run", 0.1f);

	skNode->setSkin("v1");
	skNode->setAttachment("jetpack", "jetpack");

	this->addChild(skNode);
}

void Player::setupBoundary()
{
	boundary.shape = SHAPE::rectangle;
	boundary.active = true;
	boundary.height = 75;
	boundary.width = 30;
	boundary.offset = Vec2(-15, 10);
}

void Player::update(float dt, TMXTiledMap * tiledMap)
{
	if (!isHurt)
	{
		doTerrain();
		doPhysics(dt);
	}
	bot->update(this->getPosition(), dt);
}

void Player::doTerrain()
{
	Vec2 leftTopTilePosition;
	Vec2 rightTopTilePosition;
	Vec2 leftBottomTilePosition;
	Vec2 rightBottomTilePosition;
	Vec2 leftMiddleTilePosition;
	Vec2 rightMiddleTilePosition;

	if (this->getPosition().y <= 0)
	{
		this->gotHurt();
		return;
	}

	//get corners and mid in points
	auto leftTop = this->getPosition() + Vec2(-WIDTH / 2, HEIGHT);
	auto rightTop = this->getPosition() + Vec2(WIDTH / 2, HEIGHT);
	auto leftBottom = this->getPosition() + Vec2(-WIDTH / 2, 0);
	auto rightBottom = this->getPosition() + Vec2(WIDTH / 2, 0);
	auto rightMiddle = this->getPosition() + Vec2(WIDTH / 2, HEIGHT / 2);
	auto leftMiddle = this->getPosition() + Vec2(-WIDTH / 2, HEIGHT / 2);

	//get the tiles in the corners
	leftTopTilePosition = getTileCoordinate(leftTop);
	rightTopTilePosition = getTileCoordinate(rightTop);
	leftBottomTilePosition = getTileCoordinate(leftBottom);
	rightBottomTilePosition = getTileCoordinate(rightBottom);
	leftMiddleTilePosition = getTileCoordinate(leftMiddle);
	rightMiddleTilePosition = getTileCoordinate(rightMiddle);

	//Horizontal detection
	detectRight(rightTopTilePosition, rightBottomTilePosition, rightMiddleTilePosition);
	detectLeft(leftTopTilePosition, leftBottomTilePosition, leftMiddleTilePosition);

	//Vertical detection
	detectCeiling(leftTopTilePosition, rightTopTilePosition);
	detectFloor(leftBottomTilePosition, rightBottomTilePosition);
}

Vec2 Player::getTileCoordinate(Vec2 position)
{
	int x = position.x / scaledTileWidth;
	int y = ((tiledMap->getMapSize().height * scaledTileHeight) - position.y) / scaledTileHeight;
	return Vec2(x, y);
}

void Player::detectRight(Vec2 rightTopTilePosition, Vec2 rightBottomTilePosition, Vec2 rightMiddleTilePosition)
{
	int i = rightTopTilePosition.x;
	int j = rightTopTilePosition.y;

	while (i < tiledMap->getMapSize().width)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			wall_right = propLayer->getPositionAt(Vec2(i, j)).x;
			break;
		}
		i++;
	}
	i = rightBottomTilePosition.x;
	j = rightBottomTilePosition.y;

	while (i < tiledMap->getMapSize().width)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			wall_right = std::min(propLayer->getPositionAt(Vec2(i, j)).x, wall_right);
			break;
		}
		i++;
	}
	i = rightMiddleTilePosition.x;
	j = rightMiddleTilePosition.y;

	while (i < tiledMap->getMapSize().width)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			wall_right = std::min(propLayer->getPositionAt(Vec2(i, j)).x, wall_right);
			break;
		}
		i++;
	}
}

void Player::detectLeft(Vec2 leftTopTilePosition, Vec2 leftBottomTilePosition, Vec2 leftMiddleTilePosition)
{
	int i = leftTopTilePosition.x;
	int j = leftTopTilePosition.y;
	if (i == 0)
	{
		wall_left = 0;
	}
	while (i >= 0)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			wall_left = propLayer->getPositionAt(Vec2(i, j)).x + scaledTileWidth;
			break;
		}
		i--;
	}
	i = leftBottomTilePosition.x;
	j = leftBottomTilePosition.y;
	while (i >= 0)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			wall_left = std::max(propLayer->getPositionAt(Vec2(i, j)).x + scaledTileWidth, wall_left);
			break;
		}
		i--;
	}
	i = leftMiddleTilePosition.x;
	j = leftMiddleTilePosition.y;
	while (i >= 0)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			wall_left = std::max(propLayer->getPositionAt(Vec2(i, j)).x + scaledTileWidth, wall_left);
			break;
		}
		i--;
	}
}

void Player::detectCeiling(Vec2 leftTopTilePosition, Vec2 rightTopTilePosition)
{
	int i = rightTopTilePosition.x;
	int j = rightTopTilePosition.y;
	while (j >= 0)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			ceiling = propLayer->getPositionAt(Vec2(i, j)).y;
			break;
		}
		j--;
	}
	i = leftTopTilePosition.x;
	j = leftTopTilePosition.y;
	while (j >= 0)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			ceiling = std::min(propLayer->getPositionAt(Vec2(i, j)).y, ceiling);
			break;
		}
		j--;
	}
}

void Player::detectFloor(Vec2 leftBottomTilePosition, Vec2 rightBottomTilePosition)
{
	int i = rightBottomTilePosition.x;
	int j = rightBottomTilePosition.y;

	bool footGrounded = true; //we cehck first the right foot, this will fix a little issue
	while (j < tiledMap->getMapSize().height)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			floor = propLayer->getPositionAt(Vec2(i, j)).y + scaledTileHeight;
			footGrounded = true;
			break;
		}
		if (j == tiledMap->getMapSize().height - 1)
		{
			floor = -128;

		}
		j++;
	}
	i = leftBottomTilePosition.x;
	j = leftBottomTilePosition.y;
	while (j < tiledMap->getMapSize().height)
	{
		if (propLayer->getTileGIDAt(Vec2(i, j)) == kSolidTile)
		{
			floor = std::max(propLayer->getPositionAt(Vec2(i, j)).y + scaledTileHeight, floor);
			break;
		}
		if (j == tiledMap->getMapSize().height - 1 && !footGrounded)
		{
			floor = -128;
		}
		j++;
	}
}

void Player::doPhysics(float dt)
{
	float y = this->getPosition().y;
	float x = this->getPosition().x;
	float finalX = x;
	float finalY = y;
	switch (phyState) {
	case PHYSICS_STATE::air:
		verticalSpeed += GRAVITY * dt;
		finalY = y + verticalSpeed * dt;
		if ((finalY > (ceiling - HEIGHT)) && (ceiling > 0))
		{
			verticalSpeed = 0;
			finalY = ceiling - HEIGHT;
		}
		break;
	case PHYSICS_STATE::ground:
		if (floor + 1 < y)
		{
			phyState = PHYSICS_STATE::air;
		}

	default:
		break;
	}
	if ((finalY <= floor) && (floor > 0))
	{
		finalY = floor + 1;
		phyState = PHYSICS_STATE::ground;
		verticalSpeed = 0;
		jumpCounter = jumpsEnabled;
	}
	//Horizontal movement
	double frictionConstant = (horizontalSpeed > 0) ? (-groundFriction * dt) : (groundFriction * dt);
	if (horizontalSpeed == 0)
	{
		frictionConstant = 0;
	}
	if (phyState == PHYSICS_STATE::ground)
	{
		horizontalSpeed += horizontalAccel*dt + frictionConstant;
	}
	else
		horizontalSpeed = horizontalSpeed + horizontalAccel*dt;

	if (std::abs(horizontalSpeed) < 1)
	{
		horizontalSpeed = 0;
	}
	if (std::abs(horizontalSpeed) > kMaxSpeed)
	{
		horizontalSpeed = (horizontalSpeed > 0) ? kMaxSpeed : -kMaxSpeed;
	}
	finalX = x + horizontalSpeed * dt;

	if ((finalX <= wall_left + WIDTH / 2) && (wall_left >= 0))
	{
		finalX = wall_left + WIDTH / 2;
		horizontalSpeed = 0;
	}
	if ((finalX >= wall_right - WIDTH / 2 - 1) && (wall_right > 0))
	{
		finalX = wall_right - WIDTH / 2 - 1;
		horizontalSpeed = 0;
	}

	this->doAnimation();
	this->setPosition(Vec2(finalX, finalY));
}

void Player::doAnimation()
{
	if (horizontalAccel > 0)
	{
		this->setScaleX(1);
	}
	else if (horizontalAccel < 0)
	{
		this->setScaleX(-1);
	}

	switch (phyState) {
	case PHYSICS_STATE::air:
		if (verticalSpeed > 0)
		{
			if (animState != ANIMATION_STATE::jump_up)
			{
				skNode->setAnimation(0, "jump_up", false);
				animState = ANIMATION_STATE::jump_up;
			}
		}
		else
		{
			if (animState != ANIMATION_STATE::jump_down)
			{
				skNode->setAnimation(0, "jump_down", false);
				animState = ANIMATION_STATE::jump_down;
			}
		}
		break;
	case PHYSICS_STATE::ground:
		if (std::abs(horizontalSpeed) > 40)
		{
			if (animState != ANIMATION_STATE::run && animState != ANIMATION_STATE::walk)
			{
				skNode->setAnimation(0, "walk", true);
				animState = ANIMATION_STATE::walk;
			}
			if (animState == ANIMATION_STATE::walk && std::abs(horizontalSpeed) > kRUN_SPEED)
			{
				skNode->setAnimation(0, "run", true);
				animState = ANIMATION_STATE::run;
			}
			if (animState == ANIMATION_STATE::run && std::abs(horizontalSpeed) <= kRUN_SPEED)
			{
				skNode->setAnimation(0, "walk", true);
				animState = ANIMATION_STATE::walk;
			}

		}
		else
		{
			if (animState != ANIMATION_STATE::idle)
			{
				skNode->setAnimation(0, "idle", true);
				animState = ANIMATION_STATE::idle;
			}
		}

		break;

	default:
		break;
	}
}

void Player::gotHurt()
{
	wall_left = -1;
	wall_right = -1;
	ceiling = -1;
	floor = -1;
	isHurt = true;
	horizontalAccel = 0;
	horizontalSpeed = 0;
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("sfx/hurt.wav");
	this->horizontalAccel = 0;
	this->horizontalAccel = 0;
	this->verticalSpeed = 0;

	bot->bubbleMode(this->getPosition());

	auto delayAction = DelayTime::create(0.3f);
	auto moveAction = MoveTo::create(0.3f, spawnPoint);
	auto callbackF = CallFunc::create([&]() {
		this->isHurt = false;
		bot->normalMode();
	});
	auto seq = Sequence::create(delayAction, moveAction, callbackF, NULL);
	this->stopAllActions(); //in case player got hit multiple times.. in the way back
	this->runAction(seq);
}

void Player::accelerateHorizontal(bool direction)
{
	horizontalAccel = (direction) ? FORCE_HI : -FORCE_HI;
	horizontalSpeed += (direction) ? 75 : -75;
}

void Player::pausePlayer()
{
	horizontalAccel = 0;
	horizontalSpeed = 0;
	doAnimation();
}

void Player::spawn()
{
	wall_left = -1;
	wall_right = -1;
	ceiling = -1;
	floor = -1;
	setPosition(spawnPoint);
	horizontalAccel = 0;
	horizontalSpeed = 0;
}

void Player::jump()
{
	if (phyState == PHYSICS_STATE::air && jumpCounter <= 0)
	{
		return;
	}
	auto audio = SimpleAudioEngine::getInstance();
	if (phyState == PHYSICS_STATE::air) // animate the double jump
	{
		audio->playEffect("sfx/jetpack.wav");
		jp->resetSystem();
		skNode->setAnimation(0, "jump_down", false);
		animState = ANIMATION_STATE::jump_down;
	}

	audio->playEffect("sfx/jump.wav");
	phyState = PHYSICS_STATE::air;
	verticalSpeed = 400;
	jumpCounter--;
}


