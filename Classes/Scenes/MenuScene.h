/*
 * MenuScene.h
 *
 *  Created on: 1.3.2014
 *      Author: user
 */

#ifndef MENUSCENE_H_
#define MENUSCENE_H_

#include "cocos2d.h"

class MenuScene: public cocos2d::CCScene
{
public:
    MenuScene(void);
	virtual ~MenuScene(void);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void DisplayScene(CCObject* pSender);
    void displayTutorial(CCObject* pSender);
    void closeTutorial(CCObject* pSender);

    CREATE_FUNC(MenuScene);

    CC_SYNTHESIZE(cocos2d::CCSprite*, _tutorialBG, TutorialBG);
    CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _menuBackButton, MenuBackButton);
    CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _playButton, PlayButton);
    CC_SYNTHESIZE(cocos2d::CCMenuItemImage*, _controlsButton, ControlsButton);

    CC_SYNTHESIZE(cocos2d::CCParticleSystemQuad*, _cherryParticles, CherryParticles);
    CC_SYNTHESIZE(cocos2d::CCParticleSystemQuad*, _fenemyParticles, FenemyParticles);
};


#endif /* MENUSCENE_H_ */
