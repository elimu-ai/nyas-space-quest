//
//  LoadingScene.cpp
//  akua
//
//  Created by German Torres on 11/18/15.
//
//

#include "LoadingScene.h"
#include "MarsScene.h"

Scene * Loading::createScene(int sceneNumber)
{
	auto scene = Scene::create();
	auto layer = Loading::create();
	layer->sceneNumber = sceneNumber;
	scene->addChild(layer);
	return scene;
}

bool Loading::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("loading.plist");
	auto bgSprite = Sprite::createWithSpriteFrameName("loading.png");
	bgSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/2));
	this->addChild(bgSprite);

	this->scheduleOnce(SEL_SCHEDULE(&Loading::loadLvl), 1.0);
	return true;
}

void Loading::loadLvl(float dt)
{
	Scene * scene;
	if (sceneNumber == kNumberIdA)
	{
		scene = Mars::createScene();
	}
	Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(0.5, scene));
}
