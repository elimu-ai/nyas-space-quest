//
//  NumberDisplay.h
// Nya
//
//  Created by German Torres on 7/30/15.
//  object for displaying tips 
//

#ifndef __akua__Tip__
#define __akua__Tip__

#include "BaseObject.h"
#include "androidFix.h"

class NumberDisplay : public BaseObject
{
public:
	static NumberDisplay * create(int number, Sprite * bg);
	void update(bool hit);
	void playAudio();

	bool consumed;

private:
	~NumberDisplay();
	bool isMessagevisible = false;
	void initNumberDisplay(int _number, Sprite * _bg);
	void setupBoundary();

	void setupDirector();
	void setupAudio();
	void setupSprite();
	void setupLabel();

	Vec2 origin;
	Size visibleSize;
	Size winSize;
	Size frameSize;

	Sprite * bg;
	int number;
	Label * numberLabel;
	std::string langCode;
};

#endif /* defined(__akua__BitacoraScene__) */