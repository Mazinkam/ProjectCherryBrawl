/*
 * GameScene.h
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "../Layers/GameLayer.h"
#include "../Layers/HudLayer.h"

class GameScene: public cocos2d::CCScene
{
public:
	GameScene(void);
	virtual ~GameScene(void);

	virtual bool init();

	void DisplayScene(CCObject* pSender);


	static cocos2d::CCScene* scene();

	CREATE_FUNC(GameScene);

	CC_SYNTHESIZE(GameLayer*, _gameLayer, GameLayer);
	CC_SYNTHESIZE(HudLayer*, _hudLayer, HudLayer);

};

#endif /* GAMESCENE_H_ */
