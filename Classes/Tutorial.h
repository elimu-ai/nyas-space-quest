//
//  Tutorial.h
//  Nya
//
//  Created by German Torres on 10/27/15.
//
//	

#ifndef __akua__Tutorial__
#define __akua__Tutorial__

#include "BaseObject.h"

class Tutorial : public BaseObject
{
public:
	static Tutorial * create();


private:
	~Tutorial();
	void initTutorial();

	void setupDirector();
	void setupMovement();
	void setupJump();

	Vec2 origin;
	Size visibleSize;
	Size winSize;
	Size frameSize;
};

#endif /* defined(__akua__End__) */