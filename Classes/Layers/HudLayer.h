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
		CREATE_FUNC (HudLayer);
		CC_SYNTHESIZE(SimpleDPad*, _dPad, DPad);

};

#endif /* HUDLAYER_H_ */
