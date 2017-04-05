//
//  BaseObject.h
//  Nya
//
//  Created by German Torres on 9/8/15.
//
//

#ifndef __akua__BaseObject__
#define __akua__BaseObject__

#include "cocos2d.h"
#include "kConstants.h"

USING_NS_CC;
enum SHAPE
{
	circle,
	rectangle
};

class BaseObject : public Node
{
public:
	struct BOUNDARY {
		SHAPE shape = SHAPE::circle;
		bool active = true;	//is this collidable
		float r = 50;		//radius if circle
		float width = 50;
		float height = 50;
		Vec2 offset = Vec2(0, 0);    //offset from anchor point
	} boundary;
	Color4F drwColor = Color4F::GREEN;

	void initObject();
	bool checkIntersect(BaseObject * otherObject);
private:
	DrawNode * drwNode; //this is only for debugging

	bool checkCircleCircle(BaseObject * cA, BaseObject * cB);
	bool checkCircleRect(BaseObject * c, BaseObject * r);
	bool checkRectRect(BaseObject * rA, BaseObject * rB);
	void reDraw(float dt);
};


#endif /* defined(__akua__BaseObject__) */
