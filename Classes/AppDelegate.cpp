/*
 #include "AppDelegate.h"

 #include <vector>
 #include <string>

 #include "HelloWorldScene.h"
 #include "AppMacros.h"

 USING_NS_CC;
 using namespace std;

 AppDelegate::AppDelegate() {

 }

 AppDelegate::~AppDelegate()
 {
 }

 bool AppDelegate::applicationDidFinishLaunching() {
 // initialize director
 CCDirector* pDirector = CCDirector::sharedDirector();
 CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

 pDirector->setOpenGLView(pEGLView);
 CCSize frameSize = pEGLView->getFrameSize();

 // Set the design resolution
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
 pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
 #else
 pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
 #endif


 vector<string> searchPath;

 // In this demo, we select resource according to the frame's height.
 // If the resource size is different from design resolution size, you need to set contentScaleFactor.
 // We use the ratio of resource's height to the height of design resolution,
 // this can make sure that the resource's height could fit for the height of design resolution.

 // if the frame's height is larger than the height of medium resource size, select large resource.
 if (frameSize.height > mediumResource.size.height)
 {
 searchPath.push_back(largeResource.directory);

 pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
 }
 // if the frame's height is larger than the height of small resource size, select medium resource.
 else if (frameSize.height > smallResource.size.height)
 {
 searchPath.push_back(mediumResource.directory);

 pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
 }
 // if the frame's height is smaller than the height of medium resource size, select small resource.
 else
 {
 searchPath.push_back(smallResource.directory);

 pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
 }


 // set searching path
 CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);

 // turn on display FPS
 pDirector->setDisplayStats(true);

 // set FPS. the default value is 1.0/60 if you don't call this
 pDirector->setAnimationInterval(1.0 / 60);

 // create a scene. it's an autorelease object
 CCScene *pScene = HelloWorld::scene();

 // run
 pDirector->runWithScene(pScene);

 return true;
 }

 // This function will be called when the app is inactive. When comes a phone call,it's be invoked too
 void AppDelegate::applicationDidEnterBackground() {
 CCDirector::sharedDirector()->stopAnimation();

 // if you use SimpleAudioEngine, it must be pause
 // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
 }

 // this function will be called when the app is active again
 void AppDelegate::applicationWillEnterForeground() {
 CCDirector::sharedDirector()->startAnimation();

 // if you use SimpleAudioEngine, it must resume here
 // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
 }*/
#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "Scenes/SplashScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	// for future tweaking of screensize, for now im keeping it like it is.
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

	CCEGLView *ev = CCEGLView::sharedOpenGLView();
	ev->setDesignResolutionSize(800, 480, kResolutionShowAll);

	// turn on display FPS
	pDirector->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

//	 create a scene. it's an autorelease object
    CCScene *pScene = SplashScene::scene();

    // run
    pDirector->runWithScene(pScene);


	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

