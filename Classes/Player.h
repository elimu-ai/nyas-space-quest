//
//  PlayerNode.h
//  scrollerTest.x
//
//  Created by German Torres on 7/7/15.
//
//

#ifndef __scrollerTest_x__Player__
#define __scrollerTest_x__Player__

#include "BaseObject.h"
#include "Bot.h"

#include <spine/spine-cocos2dx.h>

USING_NS_CC;

class Player : public BaseObject
{
public:
	static Player * create(TMXTiledMap * tmap);

	void update(float dt, TMXTiledMap * tiledMap);
	void jump();
	void spawn();
	void gotHurt();
	void accelerateHorizontal(bool direction);
	void pausePlayer();

	//Definition of the states
	enum PHYSICS_STATE
	{
		ground,
		air
	};
	PHYSICS_STATE phyState;

	enum ANIMATION_STATE
	{
		idle,
		walk,
		run,
		jump_up,
		jump_down
	};
	ANIMATION_STATE animState;

	//variables
	float groundFriction = 120;
	float floor = -1;
	float ceiling = -1;
	float wall_left = -1;
	float wall_right = -1;
	float horizontalAccel = 0.0f;
	Vec2 spawnPoint;
	bool isHurt;
	Bot * bot;

	//constants
	float GRAVITY = -800;
	float FORCE_LOW = 8.5;
	float FORCE_HI = 300;
	//float RUN_SPEED = 150;
	float HEIGHT = 90;
	float WIDTH = 45;

private:
	~Player();
	void initPlayer(TMXTiledMap * tmap);
	void setupAudio();
	void setupJetParticle();
	void setupSkNode();
	void setupBoundary();
	void doTerrain();
	void detectCeiling(Vec2 leftTopTilePosition, Vec2 rightTopTilePosition);
	void detectFloor(Vec2 leftBottomTilePosition, Vec2 rightBottomTilePosition);
	void detectRight(Vec2 rightTopTilePosition, Vec2 rightBottomTilePosition, Vec2 rightMiddleTilePositon);
	void detectLeft(Vec2 leftTopTilePosition, Vec2 leftBottomTilePosition, Vec2 leftMiddleTilePositon);
	void doPhysics(float dt);
	void doAnimation();

	//aux func
	Vec2 getTileCoordinate(Vec2 position);

	//variables
	float scaledTileHeight;
	float scaledTileWidth;
	float verticalSpeed = 0.0f;
	float horizontalSpeed = 0.0f;
	TMXTiledMap * tiledMap;
	TMXLayer * propLayer;
	spine::SkeletonAnimation * skNode;
	ParticleMeteor * jp;
	int jumpCounter;
	int jumpsEnabled;
};

#endif /* defined(__scrollerTest_x__Player__) */
