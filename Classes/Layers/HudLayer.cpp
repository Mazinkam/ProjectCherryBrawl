/*
 * HudLayout.cpp
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#include "HudLayer.h"
#include "../Defines.h"

using namespace cocos2d;

HudLayer::HudLayer(void)
{
	_dPad = NULL;
	_manaPool = NULL;
	_hpPool = NULL;
	_gameDialouge = NULL;
	_hpBackground = NULL;
	_hpBorder = NULL;
	_skillOne = NULL;
	_skillTwo = NULL;
	_skillThree = NULL;
	_skillFour = NULL;
}

HudLayer::~HudLayer(void)
{
}

bool HudLayer::init()
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!CCLayer::init());

		_manaPool = new CCArray;
		_hpPool = new CCArray;

		_dPad = SimpleDPad::dPadWithFile(CCString::create("DPAD_background.png"), 75);
		_dPad->setPosition(ccp(95.0, 95.0));

		_dialougeBox = CCSprite::create("dialougebox.png");
		CC_BREAK_IF(!_dialougeBox);
		_dialougeBox->setPosition(ccp(SCREEN.width/2, SCREEN.height/5));

		_hpBackground = CCSprite::create("HP_background.png");
		CC_BREAK_IF(!_hpBackground);
		_hpBackground->setPosition(ccp(SCREEN.width/2 - s_horizPadding, SCREEN.height/4.8));

		_hpBorder = CCSprite::create("HP_border.png");
		CC_BREAK_IF(!_hpBorder);
		_hpBorder->setPosition(ccp(SCREEN.width/2 - s_horizPadding, SCREEN.height/4.8));

		_gameDialouge = CCSprite::create("dialougebox001.png");
		CC_BREAK_IF(!_gameDialouge);
		this->addChild(_gameDialouge, 60);

		_gameDialouge->setVisible(false);

		this->addChild(_dialougeBox, 6);

		this->addChild(_hpBackground, 6);
		this->addChild(_hpBorder, 8);

		this->addChild(_dPad, 8);

		bRet = true;
	} while (0);

	return bRet;
}

void HudLayer::dialougeModeOn()
{
//	_dialougeBox->setVisible(false);

	if (_dPad != NULL)
		_dPad->setVisible(false);
	if (_hpBackground != NULL)
		_hpBackground->setVisible(false);
	if (_hpBorder != NULL)
		_hpBorder->setVisible(false);

	if (_gameDialouge != NULL)
		_gameDialouge->setVisible(true);
	if (_skillOne != NULL)
		_skillOne->setVisible(false);
	if (_skillTwo != NULL)
		_skillTwo->setVisible(false);
	if (_skillThree != NULL)
		_skillThree->setVisible(false);
	if (_skillFour != NULL)
		_skillFour->setVisible(false);

}
void HudLayer::dialougeModeOff()
{
//	_dialougeBox->setVisible(true);
	if (_dPad != NULL)
		_dPad->setVisible(true);
	if (_hpBackground != NULL)
		_hpBackground->setVisible(true);
	if (_hpBorder != NULL)
		_hpBorder->setVisible(true);

	if (_gameDialouge != NULL)
		_gameDialouge->setVisible(false);
	if (_skillOne != NULL)
		_skillOne->setVisible(true);
	if (_skillTwo != NULL)
		_skillTwo->setVisible(true);
	if (_skillThree != NULL)
		_skillThree->setVisible(true);
	if (_skillFour != NULL)
		_skillFour->setVisible(true);
}

void HudLayer::speechDialouge()
{

//	if (sceneOne <= 7)
//	{
//		sceneOne++;
//
//		if (sceneOne == 1)
//		{
//
//			_dialougeCherry1->setPosition(ccp(SCREEN.width/2, SCREEN.height/5));
//			_dialougeCherry1->setVisible(true);
//
//		}
//		if (sceneOne == 2)
//		{
//			_dialougeCherry1->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox002.png"));
//
//		}
//		if (sceneOne == 3)
//		{
//			_dialougeCherry1->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox003.png"));
//
//		}
//		if (sceneOne == 4)
//		{
//			_dialougeCherry1->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox004.png"));
//
//		}
//		if (sceneOne == 5)
//		{
//			_dialougeCherry1->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox005.png"));
//
//		}
//		if (sceneOne == 6)
//		{
//			_dialougeCherry1->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox006.png"));
//
//		}
//		if (sceneOne == 7)
//		{
//			_dialougeCherry1->setVisible(false);
//			checkPointOne = true;
//		}
//		LOG("%i",sceneOne);
//
//	}
//	if (sceneOne > 10 && sceneTwo < 10 && checkPointOne)
//	{
//		sceneTwo++;
//	}
//	if (sceneOne > 6)
//	{
////		_cherryText1->updateLabel();
////		_cherryText1->setString("frist labeawfawfawfawfawfawfawfl");
//	}
}

