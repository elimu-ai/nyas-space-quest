#ifndef __akua__Planet__
#define __akua__Planet__

#include "BaseObject.h"
#include "androidFix.h"

class Planet : public BaseObject
{
public:
	static Planet * create();


private:
	~Planet();
	void initPlanet();
};
#endif

