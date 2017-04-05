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

class NumberDisplay : public BaseObject
{
public:
	static NumberDisplay * create(std::string tipName, Sprite * bg);
	void update(bool hit);
	void playAudio();

	bool consumed;

private:
	~NumberDisplay();
	void initNumberDisplay(std::string tipName, Sprite * _bg);
	void setupBoundary();
	void setupMessageSprite(std::string tipName);
	void setupAudio(std::string tipName);
	void setupSprite();
	std::string name;
	bool isMessagevisible;
	Sprite * messageSprite;
	Sequence * popUpSeq;
	Sequence * popDownSeq;
	Sprite * bg;
};

#endif /* defined(__akua__BitacoraScene__) */