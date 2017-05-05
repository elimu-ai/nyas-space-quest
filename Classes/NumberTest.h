//
//  NumberTest.h
//  Nya
//
//  Created by German Torres on 4/14/17.
//  
//

#ifndef __nya__NumberTest__
#define __nya__NumberTest__

#include "BaseObject.h"
#include "androidFix.h"

class NumberTest : public BaseObject
{
public:
	static NumberTest * create(int number, Sprite * bg);
	void update(bool hit);
	void playAudio();
	static bool tutActive;
	bool consumed;

private:
	~NumberTest();
	bool isMessagevisible = false;
	void initNumberTest(int _number, Sprite * _bg);
	void setupBoundary();

	void setupDirector();
	void setupAudio();
	void setupSprite();
	void setupLabel();
	void addTut(Vec2 position);

	Vec2 origin;
	Size visibleSize;
	Size winSize;
	Size frameSize;

	Sprite * bg;
	int number;
	Label * numberLabel;
	std::string langCode;
};

#endif