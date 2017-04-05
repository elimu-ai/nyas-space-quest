//
//  Bot.h
//  Nya
//
//  Created by German Torres on 7/31/15.
//  bot bot character
//

#ifndef __akua__Bot__
#define __akua__Bot__

#include "BaseObject.h"

class Bot : public BaseObject
{
public:
	static Bot * create();
	void update(Vec2 playerPosition, float dt);
	void bubbleMode(Vec2 playerPosition);
	void normalMode();

private:
	~Bot();
	void initBot();
	void setBotPosition(float dt, Vec2 dPos);
	void setBotYPosition(float dt, float dy);
	void setBotXPosition(float dt, float dx);

	Sprite * botSprite;
	bool bubbleModeEnabled;

	float t1 = 0.0f;
	float t2 = 0.0f;
	bool isOpen = false;
};

#endif /* defined(__akua__BitacoraScene__) */