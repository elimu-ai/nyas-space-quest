//
//  GameScene.h
//  
//
//  Created by German Torres on 7/7/15.
//
//

#ifndef scrollerTest_x_GameScene_h
#define scrollerTest_x_GameScene_h

#include "cocos2d.h"
#include "Player.h"
#include "Tip.h"
#include "Coin.h"
#include "End.h"

#include "androidFix.h"

#include <spine/spine-cocos2dx.h>

USING_NS_CC;

class GameMap : public Layer
{
public:
    static Scene* createScene(std::string map);
	void setupDirector();
	void setupGameplayNode();
	void setupTilemap(std::string map);
	void setupButtonExit();
	void setupLabels();
	void setupParallax();
	void setupKeyListener();
	virtual bool init();
    void menuCloseCallback(Ref* pSender);

	virtual void loadMap();
    void update(float dt);
	void parallaxMove();
	void writeData();
	void showTotal();
	void hurtPlayer();
   
	virtual void actionKeyPressed();
	virtual void sceneExit();

	void setZoom(float z);

    CREATE_FUNC(GameMap);
    
	Vec2 origin;
    Size visibleSize;
    Size winSize;
    Size frameSize;
    
    Player * player;
	Node * gameplayNode;
    Node * parallaxNodeBack;
    Node * parallaxNodeFront;
	TMXTiledMap * tiledMap;
	TMXLayer * propLayer;
	Sprite * nave;
    End * endObject;
    bool endEnabled;
	bool isCameraActive;
	int levelId = 0;
	float zoomFactor = 0.85f;
	bool verticalParallaxEnabled = true;
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *e);
    
private:
    void centerCamera(Vec2 position);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
    std::map<EventKeyboard::KeyCode, int > keys;
    int horizontalKeysActive = 0;
	bool onTouchBegan(Touch * touch, Event * event);
    void onTouchMoved(Touch * touch, Event * event);
    void onTouchEnded(Touch * touch, Event * event);
    
    
    
    Sprite * joystickSprite;
	ParticleGalaxy * spawnMarker;
    
    Label * scoreLabel;
    float totalCoins = 0;
    int grabbedCoins = 0;
    
    Label * tipsLabel;
    float totalTips = 0;
    int grabbedTips = 0;

    float previousGPNx = 0.0f;
    float previousGPNy = 0.0f;

	float pauseTimer = 0.0f;
    int joystickActive = 0;

	//tips
	Vector<Tip*> tipVector;
	//coins
    Vector<Coin*> coinVector;
	//Tip texts
	Vector<Sprite*> tiptextVector;
};

#endif // __GAME_SCENE_H__
