/*
 * HudLayer.h
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#ifndef HUDLAYER_H_
#define HUDLAYER_H_

#include "cocos2d.h"
#include "../GameObjects/GameDPad.h"
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
		void everythingOn();

		void cherryTalks(bool isTalking, int chosenFrame);
		void fenemyTalks(bool isTalking, int chosenFrame);

		CREATE_FUNC (HudLayer);
		CC_SYNTHESIZE(GameDPad*, _dPad, DPad);

		CC_SYNTHESIZE(cocos2d::CCSprite*, _dialougeBox , DialougeBox);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _hpBackground, HpBackground);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _hpBorder, HpBorder);

		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameDialouge, GameDialouge);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameDisplayCherry, GameDisplayCherry);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameDisplayOther, GameDisplayOther);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameDisplayMauve, GameDisplayMauve);

		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameCherryNameTag, DisplayCherryNameTag);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameOtherNameTag, DisplayOtherNameTag);
		CC_SYNTHESIZE(cocos2d::CCSprite*, _gameExplanation, DisplayGameExplain);

		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _goBack , GoBack);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _tapToContinue , TapToContinue);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillOne , SkillOne);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillTwo , SkillTwo);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillThree , SkillThree);
		CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _skillFour , SkillFour);

		CC_SYNTHESIZE(cocos2d::CCArray*, _hpPool, HpPool);
		CC_SYNTHESIZE(cocos2d::CCArray*, _manaPool, ManaPool);

};

#endif /* HUDLAYER_H_ */
