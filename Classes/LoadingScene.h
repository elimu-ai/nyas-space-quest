//
//  LoadingScene.h
// Nya
//
//  Created by German Torres on 11/18/15.
//  This is the scene for the space port thingy
//

#ifndef __akua__LoadingScene__
#define __akua__LoadingScene__

#include "cocos2d.h"

#include "kConstants.h"

USING_NS_CC;

class Loading : public Layer
{
public:
	static Scene * createScene(int sceneNumber);
	virtual bool init();
	CREATE_FUNC(Loading);

	int sceneNumber;
private:
	void loadLvl(float dt);
};

#endif /* defined(__akua__LoadingScene__) */