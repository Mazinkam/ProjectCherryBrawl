/*
 * MenuScene.cpp
 *
 *  Created on: 1.3.2014
 *      Author: user
 */

#include "MenuScene.h"
#include "GameScene.h"
#include "../Defines.h"

using namespace cocos2d;

MenuScene::MenuScene(void)
{

}

MenuScene::~MenuScene(void)
{
}

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

		CCMenuItemImage *_closeButton = CCMenuItemImage::create(s_PauseOff, s_PauseOn, this, menu_selector(MenuScene::menuCloseCallback));
		CC_BREAK_IF(!_closeButton);
		_closeButton->setPosition(ccp(SCREEN.width - 25, SCREEN.height-25));

		CCMenuItemImage *_playButton = CCMenuItemImage::create(s_PlayOff, s_PlayOn, this, menu_selector(MenuScene::DisplayScene));
		CC_BREAK_IF(!_playButton);
		_playButton->setPosition(ccp(SCREEN.width/2, SCREEN.height/3.2));

		CCMenuItemImage *_controlsButton = CCMenuItemImage::create(s_ControlsOff, s_ControlsOn, this, menu_selector(MenuScene::DisplayScene));
		CC_BREAK_IF(!_controlsButton);
		_controlsButton->setPosition(ccp(SCREEN.width/2, SCREEN.height/7));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* _pMenu = CCMenu::create(_closeButton, _playButton, _controlsButton, NULL);
		_pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(!_pMenu);

		// Add the menu to MenuScene layer as a child layer.G
		CCSprite* _menuBG = CCSprite::create(s_CherryBG);
		CC_BREAK_IF(!_menuBG);
		_menuBG->setPosition(CENTER);

		CCSprite* _menuLogo = CCSprite::create(s_CherryLogo);
		CC_BREAK_IF(!_menuLogo);
		_menuLogo->setPosition(ccp(SCREEN.width/2,SCREEN.height/1.45));

		_cherryParticles = CCParticleFlower::create();
		//	_cherryParticles->initWithTotalParticles(100);
		_cherryParticles->retain();
		_cherryParticles->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));
		_cherryParticles->setPosition(SCREEN.width/1.9, SCREEN.height/1.2);

		_fenemyParticles = CCParticleFlower::create();
		//	_fenemyParticles->initWithTotalParticles(100);
		_fenemyParticles->setGravity(ccp(0,150));

		_fenemyParticles->setLifeVar(0);
		_fenemyParticles->setLife(0.7f);

		_fenemyParticles->setGravity(ccp(0,150));

		_fenemyParticles->setStartSize(0.5f);
		_fenemyParticles->setStartSizeVar(0.5f);
		_fenemyParticles->setEndSize(30.0f);
		_fenemyParticles->setEndSizeVar(5.0f);
		_fenemyParticles->retain();
		_fenemyParticles->setTexture(CCTextureCache::sharedTextureCache()->addImage("fenemyBall.png"));
		_fenemyParticles->setPosition(50, 100);

		this->addChild(_menuBG, -1);
		this->addChild(_pMenu, 1);
		this->addChild(_cherryParticles, 10);
		this->addChild(_fenemyParticles, 10);
		this->addChild(_menuLogo, 12);

		bRet = true;
	} while (0);

	return bRet;
}
void MenuScene::DisplayScene()
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, GameScene::scene(), ccBLACK));
}

void MenuScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

