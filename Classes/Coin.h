//
//  Coin.h
// Nya
//
//  Created by German Torres on 8/3/15.
//
//

#ifndef __akua__Coin__
#define __akua__Coin__

#include "BaseObject.h"

class Coin : public BaseObject
{
public:
	static Coin * create();
	bool consumed;
	void consume();

private:
	~Coin();
	void initCoin();
	void setupBoundary();
	void setupCoin();
	void setupAudio();

	Sprite * coinSprite;
	Sequence * disappear;
};

#endif /* defined(__akua__Coin__) */
