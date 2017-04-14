#ifndef __akua__Spike__
#define __akua__Spike__

#include "BaseObject.h"
#include "SimpleAudioEngine.h"

#include "androidFix.h"

class Spike : public BaseObject
{
public:
	static Spike * create();

private:
	~Spike();
	bool isAvailable;
	void initSpike();
	void setupBoundary();

	Sprite * spikeSprite;

	float speed;
	float angle;
	float speedX;
	float speedY;
};

#endif /* defined(__akua__Spike__) */