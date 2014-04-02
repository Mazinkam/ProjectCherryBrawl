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
#include "../GameObjects/EnemyBoss.h"
#include "../GameObjects/EnemyBossWings.h"
#include "../GameObjects/SimpleDPad.h"
#include "../Layers/HudLayer.h"
#include "../GameResources.h"

class GameLayer: public cocos2d::CCLayer, public SimpleDPadDelegate
{
	public:
		GameLayer(void);
		virtual ~GameLayer(void);

		CREATE_FUNC(GameLayer);

		bool init();
		void initTileMap();
		void initSingleObjects();
		void initEnemies();
		void initSkillBar();
		void initStartCutscene();
		void initStartCutsceneTwo();

		void circleSkill(CCObject* pObject);
		void firstSkill(CCObject* pObject);
		void projectileSkill(CCObject* pObject);
		void SplitSkill(CCObject* pObject);

		void bossFirstSkill();
		void bossSecondSkill();

		void keyBackClicked();
		void demoDone();

		virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
		virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::CCPoint direction);
		virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::CCPoint direction);
		virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad);

		void mainMenu(CCObject* pObject);
		void nothing(CCObject* pObject);

		void draw();

		void update(float dt);
		void updatePositions();
		void updateUI();
		void updateCutscenes(CCObject* pObject);

		void updateProjectiles();
		void updateBossProjectiles();
		void updateBoss(float dt);
		void updateEnemies(float dt);
		void setViewpointCenter(cocos2d::CCPoint position);

		void reorderActors();
		void endGame();

		void objectRemoval(cocos2d::CCNode* sender);

		void restartGame(cocos2d::CCObject* pSender);

		cocos2d::CCTMXTiledMap *_tileMap;
		cocos2d::CCSpriteBatchNode *_actorsAtlas;

	CC_SYNTHESIZE(HudLayer*, _hud, Hud)
		;CC_SYNTHESIZE(Cherry*, _cherry, Cherry)
		;CC_SYNTHESIZE(EnemyFemale*, _fenemy1, Fenemy)
		;CC_SYNTHESIZE(EnemyBoss*, _eBoss, EnemyBoss)
		;CC_SYNTHESIZE(EnemyBossWings*, _eBossWings, EnemyBossWings)
		;

	CC_SYNTHESIZE(cocos2d::CCArray*, _cherryText, CherryText)
		;CC_SYNTHESIZE(cocos2d::CCArray*, _enemyText, EnemyText)
		;CC_SYNTHESIZE(cocos2d::CCArray*, _BossText, BossText)
		;

	CC_SYNTHESIZE(cocos2d::CCArray*, _projectiles, Projectiles)
		;CC_SYNTHESIZE(cocos2d::CCArray*, _bossProjectiles, BossProjectiles)
		;

	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _enemies, Enemies);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCParticleSystemQuad*, _cherryEmitter, CherryParticleEmitter);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCParticleSystemQuad*, _bossEmitter, BossParticleEmitter);

	protected:
		bool _bInit;
		bool _dialougeState;
		int _sceneOne, _sceneTwo;
		bool _checkPointOne, _checkPointTwo, _cutsceneOneDone, _cutsceneTwoDone, _enemyBeaten, _enemyCanMove, _bFirstCheck;
		bool _checkPointOneSceneTwo, _checkPointTwoSceneTwo, _reachedBoss, _bossTalk, _bossCanMove, _bossDead,_stopCamUpdate;

};

#endif /* GAMELAYER_H_ */
