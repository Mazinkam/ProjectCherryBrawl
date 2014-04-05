/*
 * SplashScene.h
 *
 *  Created on: 1.3.2014
 *      Author: user
 */

#ifndef SPLASHSCENE_H_
#define SPLASHSCENE_H_

#include "cocos2d.h"
#include "../Defines.h"
#include "MenuScene.h"

class SplashScene: public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    void DisplayScene();

    CREATE_FUNC(SplashScene);
};



#endif /* SPLASHSCENE_H_ */
