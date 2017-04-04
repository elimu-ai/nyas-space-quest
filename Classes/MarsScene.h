//
//  MarsScene.hpp
// Nya
//
//  Created by German Torres on 12/9/15.
//
//

#ifndef __akua__MarsScene__
#define __akua__MarsScene__

#include "cocos2d.h"
#include "GameMapScene.h"
#include "LoadingScene.h"

USING_NS_CC;

class Mars : public GameMap
{
public:
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Mars);
    
private:
    void loadMap();
	void setupAudio();
	void setupBackground();
	void setupParallaxImages();
    void update(float dt);
	ParticleRain * dust;
};

#endif /* defined(__akua__MarsScene__) */

