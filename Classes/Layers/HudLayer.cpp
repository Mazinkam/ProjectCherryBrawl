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
	_gameDisplayCherry = NULL;
	_gameDisplayOther = NULL;
	_hpBackground = NULL;
	_hpBorder = NULL;
	_skillOne = NULL;
	_skillTwo = NULL;
	_skillThree = NULL;
	_skillFour = NULL;
	_tapToContinue = NULL;
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
		CC_BREAK_IF(!_manaPool);
		_hpPool = new CCArray;
		CC_BREAK_IF(!_hpPool);

		_dPad = SimpleDPad::dPadWithFile(CCString::create(s_DpadBG), 75);
		CC_BREAK_IF(!_dPad);
		_dPad->setPosition(ccp(95.0, 95.0));

		_dialougeBox = CCSprite::create(s_DialougeBox);
		CC_BREAK_IF(!_dialougeBox);
		_dialougeBox->setPosition(ccp(SCREEN.width/2, SCREEN.height/5));

		_hpBackground = CCSprite::create(s_HpBG);
		CC_BREAK_IF(!_hpBackground);
		_hpBackground->setPosition(ccp(SCREEN.width/2 - s_horizPadding, SCREEN.height/4.8));

		_hpBorder = CCSprite::create(s_HpBorder);
		CC_BREAK_IF(!_hpBorder);
		_hpBorder->setPosition(ccp(SCREEN.width/2 - s_horizPadding, SCREEN.height/4.8));

		_gameDialouge = CCSprite::create(s_Dialouge1);
		CC_BREAK_IF(!_gameDialouge);
		this->addChild(_gameDialouge, 60);

		_gameDisplayCherry = CCSprite::create(s_CherryNormal1);
		CC_BREAK_IF(!_gameDisplayCherry);
		this->addChild(_gameDisplayCherry, 59);

		_gameDisplayOther = CCSprite::create(s_FenemyNormal);
		CC_BREAK_IF(!_gameDisplayOther);
		this->addChild(_gameDisplayOther, 59);


		_gameCherryNameTag = CCSprite::create(s_CherryNameTag);
		CC_BREAK_IF(!_gameCherryNameTag);
		this->addChild(_gameCherryNameTag, 62);

		_gameOtherNameTag = CCSprite::create(s_UnknownNameTag);
		CC_BREAK_IF(!_gameOtherNameTag);
		this->addChild(_gameOtherNameTag, 62);

		_gameDialouge->setVisible(false);
		_gameDisplayCherry->setVisible(false);
		_gameDisplayOther->setVisible(false);
		_gameCherryNameTag->setVisible(false);
		_gameOtherNameTag->setVisible(false);

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
	if (_tapToContinue != NULL)
		_tapToContinue->setVisible(true);

	if (_skillOne != NULL)
		_skillOne->setVisible(false);
	if (_skillTwo != NULL)
		_skillTwo->setVisible(false);
	if (_skillThree != NULL)
		_skillThree->setVisible(false);
	if (_skillFour != NULL)
		_skillFour->setVisible(false);
	if (_skillFour != NULL)
		_skillFour->setVisible(false);
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
	if (_gameDisplayCherry != NULL)
		_gameDisplayCherry->setVisible(false);
	if (_gameDisplayOther != NULL)
		_gameDisplayOther->setVisible(false);
	if (_gameCherryNameTag != NULL)
		_gameCherryNameTag->setVisible(false);
	if (_gameOtherNameTag != NULL)
		_gameOtherNameTag->setVisible(false);
	if (_tapToContinue != NULL)
		_tapToContinue->setVisible(false);


	if (_skillOne != NULL)
		_skillOne->setVisible(true);
	if (_skillTwo != NULL)
		_skillTwo->setVisible(true);
	if (_skillThree != NULL)
		_skillThree->setVisible(true);
	if (_skillFour != NULL)
		_skillFour->setVisible(true);
}
