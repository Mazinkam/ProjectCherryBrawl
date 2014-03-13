/*
 * GameLayer.h
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#ifndef GAMELAYER_H_
#define GAMELAYER_H_

#include "cocos2d.h"
#include "../GameObjects/Cherry.h"
#include "../GameObjects/EnemyFemale.h"
#include "../GameObjects/SimpleDPad.h"
#include "../Layers/HudLayer.h"

class GameLayer: public cocos2d::CCLayer, public SimpleDPadDelegate
{
	public:
		GameLayer(void);
		virtual ~GameLayer(void);

		CREATE_FUNC(GameLayer);

		bool init();
		void initTileMap();
		void initCherry();
		void initEnemies();
		void initSkillBar();

		void circleSkill();
		void firstSkill();
		void projectileSkill();


		void keyBackClicked();


		virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
		virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::CCPoint direction);
		virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::CCPoint direction);
		virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad);

		void mainMenu();

		void draw();

		void update(float dt);
		void updatePositions();
		void updateProjectiles();
		void updateEnemies(float dt);
		void setViewpointCenter(cocos2d::CCPoint position);

		void reorderActors();

		void endGame();

		void projectileMoveFinished(cocos2d::CCNode* sender);

		void restartGame(cocos2d::CCObject* pSender);

		cocos2d::CCTMXTiledMap *_tileMap;
		cocos2d::CCSpriteBatchNode *_actorsTest;

		CC_SYNTHESIZE(HudLayer*, _hud, Hud);
		CC_SYNTHESIZE(Cherry*, _cherry, Cherry);

		CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _enemies, Enemies);

		CC_SYNTHESIZE(cocos2d::CCSprite*, _hpBlip, HpBlip);

	protected:
		bool _bInit;
	    cocos2d::CCArray *_targets;
	    cocos2d::CCArray *_projectiles;
	    cocos2d::CCParticleSystemQuad* m_emitter;

};

#endif /* GAMELAYER_H_ */
