//
//  End.h
// Nya
//
//  Created by German Torres on 10/27/15.
//
//	this object marks the end of a level and exits(?) it

#ifndef __akua__End__
#define __akua__End__

#include "BaseObject.h"

class End : public BaseObject
{
public:
	static End * create();


private:
	~End();
	void initEnd();
	void setupBoundary();
};

#endif /* defined(__akua__End__) */