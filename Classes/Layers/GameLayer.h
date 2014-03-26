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
#include "../GameResources.h"

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
		void initStartCutscene();

		void circleSkill();
		void firstSkill();
		void projectileSkill();
		void SplitSkill();

		void keyBackClicked();


		virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
		virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::CCPoint direction);
		virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::CCPoint direction);
		virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad);

		void mainMenu();

		void draw();

		void update(float dt);
		void updatePositions();
		void updateUI();
		void updateCutsceneOne();
		void updateCutsceneTwo();
		void updateProjectiles();
		void updateEnemies(float dt);
		void setViewpointCenter(cocos2d::CCPoint position);

		void reorderActors();
		void endGame();

		void objectRemoval(cocos2d::CCNode* sender);

		void restartGame(cocos2d::CCObject* pSender);

		cocos2d::CCTMXTiledMap *_tileMap;
		cocos2d::CCSpriteBatchNode *_actorsAtlas;

		CC_SYNTHESIZE(HudLayer*, _hud, Hud);
		CC_SYNTHESIZE(Cherry*, _cherry, Cherry);
		CC_SYNTHESIZE(EnemyFemale*, _fenemy1, Fenemy);

		CC_SYNTHESIZE(cocos2d::CCArray*, _cherryText, CherryText);
		CC_SYNTHESIZE(cocos2d::CCArray*, _enemyText, EnemyText);
		CC_SYNTHESIZE(cocos2d::CCArray*, _BossText, BossText);

		CC_SYNTHESIZE(cocos2d::CCArray*, _projectiles, Projectiles);

		CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _enemies, Enemies);
		CC_SYNTHESIZE_RETAIN(cocos2d::CCParticleSystemQuad*, m_emitter, ParticleSystemQuad);

	protected:
		bool _bInit;
		bool _dialougeState;
		int _sceneOne, _sceneTwo;
		bool _checkPointOne, _checkPointTwo, _enemyBeaten, _bFirstCheck;


};

#endif /* GAMELAYER_H_ */
