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
	_dPad->setVisible(false);
	_hpBackground->setVisible(false);
	_hpBorder->setVisible(false);
}
void HudLayer::dialougeModeOff()
{
//	_dialougeBox->setVisible(true);
	_dPad->setVisible(true);
	_hpBackground->setVisible(true);
	_hpBorder->setVisible(true);
}

