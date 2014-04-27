/*
 * SplashScene.cpp
 *
 *  Created on: 1.3.2014
 *      Author: user
 */

#include "MenuScene.h"
#include "SplashScene.h"
#include "cocos2d.h"

using namespace cocos2d;

CCScene* SplashScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		SplashScene *layer = SplashScene::create();
		CC_BREAK_IF(!layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{

	bool bRet = false;
	do
	{

		CC_BREAK_IF(!CCScene::init());


		CCSprite* pSprite = CCSprite::create("splash.png");
		CC_BREAK_IF(!pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(SCREEN.width/2, SCREEN.height/2));

		// Add the sprite to SplashScene layer as a child layer.
		this->addChild(pSprite, 0);

		//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, MenuScene::scene(), ccWHITE));

		CCCallFunc* changeScene = CCCallFunc::create(this, callfunc_selector(SplashScene::DisplayScene));

		CCDelayTime* delayAction = CCDelayTime::create(2.0f);

		this->runAction(CCSequence::create(delayAction, changeScene, NULL));

		bRet = true;

	} while (0);

	return true;
}

void SplashScene::DisplayScene()
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, MenuScene::scene(), ccWHITE));
}

