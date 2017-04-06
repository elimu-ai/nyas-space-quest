#include "AppDelegate.h"
/*#include "IntroScene.h"
#include "MenugScene.h"
#include "BitacoraScene.h"
#include "PuertoScene.h"
#include "LoadingScene.h"*/
#include "MenugScene.h"
#include <random>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}


typedef struct tagResource
{
	Vec2 size;
	char directory[10];
}Resource;

static Resource smallResource = { Vec2(480, 320),   "SD" };
static Resource mediumResource = { Vec2(1024, 768),  "HD" };
static Resource largeResource = { Vec2(2048, 1536), "HDR" };


static Vec2 designResolutionSize = Vec2(1024, 500);

bool AppDelegate::applicationDidFinishLaunching() {
	std::srand(time(NULL));

	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		//glview = GLViewImpl::createWithRect("NYA", Rect(0,0, 1024, 768));
		glview = GLViewImpl::createWithFullScreen("NYA");
#else
		glview = GLViewImpl::create("NYA");
		//glview = GLViewImpl::createWithFullScreen("NYA"); //works on windows
#endif
		director->setOpenGLView(glview);
	}
	glview->setDesignResolutionSize(designResolutionSize.x, designResolutionSize.y, ResolutionPolicy::FIXED_HEIGHT);
	Vec2 frameSize = glview->getFrameSize();

	// if the frame's height is larger than the height of medium resource size, select large resource.
	std::vector<std::string> searchPath;
	if (frameSize.x > mediumResource.size.x)
	{
		searchPath.push_back(largeResource.directory);
		director->setContentScaleFactor(largeResource.size.x / designResolutionSize.x);
	}
	// if the frame's height is larger than the height of small resource size, select medium resource.
	else if (frameSize.x > smallResource.size.x)
	{
		searchPath.push_back(mediumResource.directory);
		director->setContentScaleFactor(mediumResource.size.x / designResolutionSize.x);
	}
	// if the frame's height is smaller than the height of medium resource size, select small resource.
	else
	{
		searchPath.push_back(smallResource.directory);
		director->setContentScaleFactor(smallResource.size.x / designResolutionSize.x);
	}
	FileUtils::getInstance()->setSearchResolutionsOrder(searchPath);

	// turn on display FPS
	director->setDisplayStats(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	register_all_packages();

	// create a scene. it's an autorelease object
	auto scene = Menug::createScene();
	//auto scene = Menug::createScene();
	//auto scene = Bitacora::createScene();
	//auto scene = Jupiter::createScene();
	//auto scene = Puerto::createScene();
	//auto scene = Loading::createScene(kMoon);
	//auto scene = Uranus::createScene();
	// runw
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
