/*
 * MenuScene.cpp
 *
 *  Created on: 1.3.2014
 *      Author: user
 */

#include "MenuScene.h"
#include "GameScene.h"
#include "../Utils/TouchTrailLayer.h"


using namespace cocos2d;

CCScene* MenuScene::scene()
{
	CCScene * scene = NULL;
	do
	{

		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		MenuScene *layer = MenuScene::create();
		CC_BREAK_IF(!layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	bool bRet = false;
	do
	{

		CC_BREAK_IF(!CCScene::init());

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(MenuScene::menuCloseCallback));
		CC_BREAK_IF(!pCloseItem);

		// Place the menu item bottom-right conner.
		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(!pMenu);

		// Add the menu to MenuScene layer as a child layer.
		this->addChild(pMenu, 1);
		CCSize size2 = CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *pMenu1 = CCMenuItemImage::create("playOn.png", "playOff.png", this, menu_selector(GameScene::DisplayScene));
		CC_BREAK_IF(!pMenu1);
		CCMenu* pMenu11 = CCMenu::create(pMenu1, NULL);
		pMenu11->setPosition(ccp(size2.width/2, size2.height/1.2));
		CC_BREAK_IF(!pMenu11);
		this->addChild(pMenu11, 1);


		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* pSprite = CCSprite::create("PCBlogo2.png");
		CC_BREAK_IF(!pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(size.width/2, size.height/2));

		this->addChild(pSprite);
		bRet = true;
	} while (0);

	return bRet;
}

void MenuScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

