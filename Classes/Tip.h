//
//  Tip.h
//  akua
//
//  Created by German Torres on 7/30/15.
//  object for displaying tips 
//

#ifndef __akua__Tip__
#define __akua__Tip__

#include "BaseObject.h"

class Tip : public BaseObject
{
public:
	static Tip * create(std::string tipName);
	void update(bool hit);
	void playAudio();
   
	bool consumed;

private:
	~Tip();
	void initTip(std::string tipName);
	void setupBoundary();
	void setupMessageSprite(std::string tipName);
	void setupAudio(std::string tipName);
	void setupSprite();
	
	
	std::string name;
	bool isMessagevisible;
	Sprite * messageSprite;
	Sequence * popUpSeq;
	Sequence * popDownSeq;
};

#endif /* defined(__akua__BitacoraScene__) */