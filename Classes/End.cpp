#include "End.h"

End * End::create()
{
	End * end = new End();

	if (end && end->init())
	{
		end->autorelease();
		end->initEnd();
		return end;
	}
	CC_SAFE_DELETE(end);
	return NULL;
}

End::~End() {}

void End::initEnd()
{
	auto endEmitter = ParticleFlower::create();
	endEmitter->setPositionType(ParticleSystem::PositionType::GROUPED);
	endEmitter->setPosition(Vec2::ZERO);
	this->addChild(endEmitter);
	setupBoundary();
	BaseObject::initObject();
}

void End::setupBoundary()
{
	boundary.shape = circle;
	boundary.r = 40;
	boundary.active = true;
}
