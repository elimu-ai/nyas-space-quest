//
//  UranusScene.hpp
//  akua
//
//  Created by German Torres on 12/6/15.
//
//
#ifndef __akua__UranusScene__
#define __akua__UranusScene__

#include "cocos2d.h"
#include "GameMapScene.h"
#include "LoadingScene.h"
#include "Spike.h"

USING_NS_CC;

class Uranus : public GameMap
{
public:
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Uranus);
    
private:
	ParticleSnow * snow;
    void loadMap();
	void setupAudio();
	void setupBackground();
	void setupParallaxImages();
	void update(float dt);
	void setupSpikes();
	Vector<Spike*> spikeVector;
};

#endif /* defined(__akua__UranusScene__) */