//
//  BaseObject.cpp
//  Nya
//
//  Created by German Torres on 9/8/15.
//
//

#include "BaseObject.h"

USING_NS_CC;

void BaseObject::initObject()
{
#ifdef kCollisionDebug
	drwNode = DrawNode::create();
	this->addChild(drwNode);
	this->schedule(CC_SCHEDULE_SELECTOR(BaseObject::reDraw), 0.1); // update every half second. may be handy if the shape changes
#endif
}

bool BaseObject::checkIntersect(BaseObject * otherObject)
{
	if (otherObject->boundary.shape == circle && this->boundary.shape == circle)
	{
		return checkCircleCircle(this, otherObject);
	}
	if (otherObject->boundary.shape == circle && this->boundary.shape == rectangle)
	{
		return checkCircleRect(this, otherObject);
	}
	if (otherObject->boundary.shape == rectangle && this->boundary.shape == circle)
	{
		return checkCircleRect(this, otherObject);
	}
	if (otherObject->boundary.shape == rectangle && this->boundary.shape == rectangle)
	{
		return checkRectRect(this, otherObject);
	}
	return false;
}

bool BaseObject::checkCircleCircle(BaseObject * cA, BaseObject * cB)
{
	Vec2 transformedOffsetCa = cA->boundary.offset * cA->getScaleY();
	if (cA->getRotation() != 0)
	{
		transformedOffsetCa = transformedOffsetCa.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(-cA->getRotation()));
	}
	Vec2 transformedOffsetCb = cB->boundary.offset * cB->getScaleY();
	if (cB->getRotation() != 0)
	{
		transformedOffsetCb = transformedOffsetCb.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(-cB->getRotation()));
	}
	Vec2 positionA = cA->getPosition() + transformedOffsetCa;
	Vec2 positionB = cB->getPosition() + transformedOffsetCb;
	float scaleA = cA->getScaleY();
	float scaleB = cB->getScaleY();
	float distance = positionA.getDistance(positionB);
	float maxDistance = cA->boundary.r * scaleA + cB->boundary.r * scaleB;
	if (maxDistance > distance && !cB->boundary.active)
	{
		cB->drwColor = Color4F::YELLOW;
		return false;
	}
	if (maxDistance > distance)
	{
		cB->drwColor = Color4F::RED;
		return true;
	}
	else
	{
		cB->drwColor = Color4F::GREEN;
	}
	return false;
}

bool BaseObject::checkCircleRect(BaseObject *r, BaseObject *c)
{
	Vec2 transformedOffsetC = c->boundary.offset * c->getScaleY();
	if (c->getRotation() != 0)
	{
		transformedOffsetC = transformedOffsetC.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(-c->getRotation()));
	}
	Vec2 transformedOffsetR = r->boundary.offset * r->getScaleY();

	Vec2 positionR = r->getPosition() + transformedOffsetR;
	Vec2 positionC = c->getPosition() + transformedOffsetC;

	float x = positionR.x;
	float y = positionR.y;
	float width = r->boundary.width * r->getScaleY();
	float height = r->boundary.height * r->getScaleY();
	Rect rectangle = Rect(x, y, width, height);

	float rR = c->boundary.r * c->getScaleY();
	if (rectangle.intersectsCircle(positionC, rR) && !r->boundary.active)
	{
		c->drwColor = Color4F::YELLOW;
		return true;
	}

	if (rectangle.intersectsCircle(positionC, rR))
	{
		c->drwColor = Color4F::RED;
		return true;
	}
	else
	{
		c->drwColor = Color4F::GREEN;
	}
	return false;
}

bool BaseObject::checkRectRect(BaseObject *rA, BaseObject *rB)
{
	//for rectangles we only support AABB 
	Vec2 positionRa = rA->getPosition() + rA->boundary.offset * rA->getScaleY();
	float widthA = rA->boundary.width * rA->getScaleY();
	float heightA = rA->boundary.height * rA->getScaleY();
	Rect rectangleA = Rect(positionRa.x, positionRa.y, widthA, heightA);

	Vec2 positionRb = rB->getPosition() + rB->boundary.offset * rB->getScaleY();
	float widthB = rB->boundary.width * rB->getScaleY();
	float heightB = rB->boundary.height * rB->getScaleY();
	Rect rectangleB = Rect(positionRb.x, positionRb.y, widthB, heightB);

	if (rectangleA.intersectsRect(rectangleB) && !rB->boundary.active)
	{
		rB->drwColor = Color4F::YELLOW;
		return false;
	}
	if (rectangleA.intersectsRect(rectangleB))
	{
		rB->drwColor = Color4F::RED;
		return true;
	}
	else
	{
		rB->drwColor = Color4F::GREEN;
	}
	return false;
}

void BaseObject::reDraw(float dt)
{
	drwNode->clear();
	if (boundary.shape == SHAPE::circle)
	{
		//centered at anchor point + offset
		drwNode->drawCircle(boundary.offset, boundary.r, 0, 15, true, 1, 1, drwColor);
	}
	if (boundary.shape == SHAPE::rectangle)
	{
		//left bottom corner at anchor point + offset
		drwNode->drawRect(boundary.offset, Vec2(boundary.width + boundary.offset.x, boundary.height + boundary.offset.y), drwColor);
	}
}
