/*
 * HudLayer.h
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#ifndef HUDLAYER_H_
#define HUDLAYER_H_

#include "cocos2d.h"
#include "../GameObjects/SimpleDPad.h"
#include "../GameObjects/Cherry.h"
#include "../GameObjects/EnemyFemale.h"
#include "../Scenes/MenuScene.h"

class HudLayer: public cocos2d::CCLayer
{
	public:
		HudLayer(void);
		virtual ~HudLayer(void);

		bool init();
		void dialougeModeOn();
		void dialougeModeOff();

		CREATE_FUNC (HudLayer);
		CC_SYNTHESIZE(SimpleDPad*, _dPad, DPad);

		CC_SYNTHESIZE(cocos2d::CCSprite*, _dialougeBox , DialougeBox);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _hpBackground, HpBackground);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _hpBorder, HpBorder);

		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameDialouge, GameDialouge);

		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _goBack , GoBack);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillOne , SkillOne);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillTwo , SkillTwo);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillThree , SkillThree);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillFour , SkillFour);

		CC_SYNTHESIZE(cocos2d::CCArray*, _hpPool, HpPool);
		CC_SYNTHESIZE(cocos2d::CCArray*, _manaPool, ManaPool);

};

#endif /* HUDLAYER_H_ */
