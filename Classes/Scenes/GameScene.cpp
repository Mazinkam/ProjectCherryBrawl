#include "GameScene.h"
using namespace cocos2d;

GameScene::GameScene(void)
{
	_gameLayer = NULL;
	_hudLayer = NULL;
}

GameScene::~GameScene(void)
{
}

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do
	{

		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		GameScene *layer = GameScene::create();
		CC_BREAK_IF(!layer);

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}

bool GameScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCScene::init());

		_hudLayer = HudLayer::create();
		this->addChild(_hudLayer, 1);

		_gameLayer = GameLayer::create();
		this->addChild(_gameLayer, 0);

		_hudLayer->getDPad()->setDelegate(_gameLayer);

		_gameLayer->setHud(_hudLayer);

		bRet = true;

	} while (0);

	return bRet;
}

void GameScene::DisplayScene(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, GameScene::scene(), ccWHITE));
}



