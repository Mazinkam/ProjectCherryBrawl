/*
 * GameDPad.h
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#ifndef SIMPLEDPAD_H_
#define SIMPLEDPAD_H_

#include "cocos2d.h"

class GameDPad;

class GameDPadDelegate
{
	public:
		virtual void didChangeDirectionTo(GameDPad *simpleDPad, cocos2d::CCPoint direction) = 0;
		virtual void isHoldingDirection(GameDPad *simpleDPad, cocos2d::CCPoint direction) = 0;
		virtual void simpleDPadTouchEnded(GameDPad *simpleDPad) = 0;
};

class GameDPad: public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
	public:
		GameDPad(void);
		virtual ~GameDPad(void);

		static GameDPad* dPadWithFile(cocos2d::CCString *fileName, float radius);
		bool initWithFile(cocos2d::CCString *filename, float radius);

		void onEnterTransitionDidFinish();
		void onExit();
		void update(float dt);

		virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
		virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
		virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

		void updateDirectionForTouchLocation(cocos2d::CCPoint location);

		CC_SYNTHESIZE(GameDPadDelegate*, _delegate, Delegate);
		CC_SYNTHESIZE(CCSprite*, _touchStick, CCSprite);
		CC_SYNTHESIZE(bool, _isHeld, IsHeld);

	protected:
		float _radius;
		cocos2d::CCPoint _direction;
};

#endif /* SIMPLEDPAD_H_ */
