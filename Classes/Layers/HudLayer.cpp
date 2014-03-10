
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
}

HudLayer::~HudLayer(void)
{
}

bool HudLayer::init()
{
	bool bRet = false;

	do
	{
		int _skillButtonSize = 76;
		int _betweenButtons = 60;
		int _sizeFromSide = 52;
		int _sidePadding = 20;
		int _horizPadding = 10;

		int _blipSize = 32;
		int _blipSpaceSize = 7;

		int _manaSize = 48;
		int _manaPadding = 15;
		int _manaSpaceSize = 15;

		int _lifeSize = 50;
		int _lifeSpaceSize = 10;

		CC_BREAK_IF(!CCLayer::init());

		_dPad = SimpleDPad::dPadWithFile(CCString::create("DPAD_background.png"), 75);
		_dPad->setPosition(ccp(95.0, 95.0));

		CCSprite *_dialougeBox = CCSprite::create("dialougebox.png");
		CC_BREAK_IF(!_dialougeBox);
		_dialougeBox->setPosition(ccp(SCREEN.width/2, SCREEN.height/5));

		CCSprite *_hpBackground = CCSprite::create("HP_background.png");
		CC_BREAK_IF(!_hpBackground);
		_hpBackground->setPosition(ccp(SCREEN.width/2 - _horizPadding, SCREEN.height/4.8));

		CCSprite *_hpBorder = CCSprite::create("HP_border.png");
		CC_BREAK_IF(!_hpBorder);
		_hpBorder->setPosition(ccp(SCREEN.width/2 - _horizPadding, SCREEN.height/4.8));

		this->addChild(_dialougeBox, 6);

		this->addChild(_hpBackground, 6);
		this->addChild(_hpBorder, 8);

		this->addChild(_dPad, 8);

		bRet = true;
	} while (0);

	return bRet;
}
