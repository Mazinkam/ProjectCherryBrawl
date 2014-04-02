#include "../Layers/GameLayer.h"
#include "../GameObjects/EnemyFemale.h"
#include "../Scenes/GameScene.h"
//#include "SimpleAudioEngine.h"
#include "../Defines.h"

using namespace cocos2d;
bool showHitBox = true;

GameLayer::GameLayer(void) :
		_projectiles(NULL), _bossProjectiles(NULL)
{
	_tileMap = NULL;
	_cherry = NULL;
	_fenemy1 = NULL;
	_eBoss = NULL;
	_eBossWings = NULL;
	_enemies = NULL;
	_bInit = false;
	_dialougeState = false;
	_enemyBeaten = true;
	_enemyCanMove = false;
	_bossCanMove = false;
	_reachedBoss = false;
	_bossDead = false;
	_checkPointOneSceneTwo = false;
	_checkPointTwoSceneTwo = false;
	_stopCamUpdate = false;

}

GameLayer::~GameLayer(void)
{
	// cpp don't need to call super dealloc
	// virtual destructor will do this
	this->unscheduleUpdate();
}

bool GameLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		// Load audio
		//		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("superd_theme.ogg");
		//		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("superd_theme.ogg", true);

		_projectiles = new CCArray;
		CC_BREAK_IF(!_projectiles);
		_bossProjectiles = new CCArray;
		CC_BREAK_IF(!_bossProjectiles);
		_cherryText = new CCArray;
		CC_BREAK_IF(!_cherryText);
		_enemyText = new CCArray;
		CC_BREAK_IF(!_enemyText);

		this->setTouchEnabled(true);

		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CC_BREAK_IF(!cache);
		cache->addSpriteFramesWithFile(s_gameAtlasPlist, s_gameAtlasPNG);

		_actorsAtlas = CCSpriteBatchNode::create(s_gameAtlasPNG);
		CC_BREAK_IF(!_actorsAtlas);
		_actorsAtlas->getTexture()->setAntiAliasTexParameters();
		this->addChild(_actorsAtlas, -5);

		this->initTileMap();
		LOG("initTileMap");
		this->initSingleObjects();
		LOG("initSingleObjects");
		this->initEnemies();
		LOG("initEnemies");

		this->scheduleUpdate();
		LOG("scheduleUpdate");
		this->setKeypadEnabled(true);

		_cherryEmitter = new CCParticleSystemQuad();
		CC_BREAK_IF(!_cherryEmitter);
		_bossEmitter = new CCParticleSystemQuad();
		CC_BREAK_IF(!_bossEmitter);

		bRet = true;
	} while (0);
	return bRet;
}

void GameLayer::initTileMap()
{
	_tileMap = CCTMXTiledMap::create(s_TilesTMX);
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_tileMap->getChildren(), pObject)
	{
		CCTMXLayer *child = (CCTMXLayer*) pObject;
		child->getTexture()->setAliasTexParameters();
	}
	this->addChild(_tileMap, -6);
}

void GameLayer::initStartCutscene()
{

	LOG("initStartCutscene");
	_sceneOne = 0;
	_sceneTwo = 0;
	_cutsceneOneDone = false;
	_cutsceneTwoDone = false;

	_checkPointOne = true;
	_checkPointTwo = false;

	_dialougeState = true;
	_enemyBeaten = false;
	_bossTalk = false;
	_bossDead = false;
	_reachedBoss = false;

	_hud->getGameDialouge()->setPosition(ccp(SCREEN.width/2, SCREEN.height/5));

	_hud->getGameDisplayCherry()->setPosition(ccp(SCREEN.width/4, SCREEN.height/1.4));
	_hud->getGameDisplayOther()->setPosition(ccp(SCREEN.width/1.3, SCREEN.height/1.4));

	_hud->getDisplayCherryNameTag()->setPosition(ccp(SCREEN.width/3.5, SCREEN.height/2.25));
	_hud->getDisplayOtherNameTag()->setPosition(ccp(SCREEN.width/1.4, SCREEN.height/2.255));

	_hud->setTapToContinue(CCMenuItemImage::create(s_TapOff, s_TapOn, this, menu_selector(GameLayer::updateCutscenes)));
	_hud->getTapToContinue()->setPosition(ccp(SCREEN.width/2, SCREEN.height/12));

	_hud->getGameDialouge()->setVisible(false);

	_hud->getGameDisplayCherry()->setVisible(false);
	_hud->getDisplayCherryNameTag()->setVisible(false);

	_hud->getGameDisplayOther()->setVisible(false);
	_hud->getDisplayOtherNameTag()->setVisible(false);

	CCMenu* pMenu = CCMenu::create(_hud->getTapToContinue(), NULL);
	pMenu->setPosition(CCPointZero);

	pMenu->setTag(61);
	_hud->addChild(pMenu, 61);

}

void GameLayer::initStartCutsceneTwo()
{
	LOG("initStartCutsceneTwo");
	_sceneTwo = 0;

	_dialougeState = true;
	_bossTalk = true;
	_bossDead = false;
	_reachedBoss = true;
}

void GameLayer::initSingleObjects()
{
	_cherry = Cherry::create();
	_actorsAtlas->addChild(_cherry);
	_cherry->setPosition(ccp(_cherry->getCenterToSides(), 80));
	_cherry->setDesiredPosition(_cherry->getPosition());
	_cherry->idle();
	LOG("Cherry done");

	_fenemy1 = EnemyFemale::create();
	_actorsAtlas->addChild(_fenemy1);
	_fenemy1->setPosition(ccp(SCREEN.width/1.5, 80));
	_fenemy1->setDesiredPosition(_fenemy1->getPosition());
	_fenemy1->setScaleX(-1);
	_fenemy1->idle();
	LOG("EnemyFemale done");

	_eBossWings = EnemyBossWings::create();
	_actorsAtlas->addChild(_eBossWings);
	_eBossWings->setPosition(ccp(_tileMap->getMapSize().width * _tileMap->getTileSize().width - _eBossWings->getCenterToSides()*2, 80));
	_eBossWings->setDesiredPosition(_eBossWings->getPosition());
	_eBossWings->setScaleX(-1);
	_eBossWings->idle();
	LOG("EnemyBossWings done");

	_eBoss = EnemyBoss::create();
	_actorsAtlas->addChild(_eBoss);
	_eBoss->setPosition(ccp(_tileMap->getMapSize().width * _tileMap->getTileSize().width - _eBoss->getCenterToSides()*2, 80));
	_eBoss->setDesiredPosition(_eBoss->getPosition());
	_eBoss->setScaleX(-1);
	_eBoss->idle();
	LOG("EnemyBoss done");

}
void GameLayer::initEnemies()
{
	int enemyCount = 1;

	this->setEnemies(CCArray::createWithCapacity(enemyCount));
	LOG("Started enemies init");
	for (int i = 0; i < enemyCount; i++)
	{
		EnemyFemale *enemy = EnemyFemale::create();
		_actorsAtlas->addChild(enemy);
		_enemies->addObject(enemy);

		int minX = SCREEN.width + enemy->getCenterToSides()*3;
		int maxX = ((_tileMap->getMapSize().width * _tileMap->getTileSize().width) * 0.8) - enemy->getCenterToSides();
		int minY = enemy->getCenterToBottom();
		int maxY = 3 * _tileMap->getTileSize().height + enemy->getCenterToBottom();

		enemy->setScaleX(-1);
		enemy->setPosition(ccp(random_range(minX, maxX), random_range(minY, maxY)));
		enemy->setDesiredPosition(enemy->getPosition());
		enemy->idle();
		LOG("%i", i);
	}
	LOG("done enemies init");

}
void GameLayer::initSkillBar()
{
	if (_hud != NULL && _hud->getChildByTag(10) == NULL && _bInit == false)
	{
		LOG("initSkillBar");
		_hud->setGoBack(CCMenuItemImage::create(s_PauseOff, s_PauseOn, this, menu_selector(GameLayer::mainMenu)));
		_hud->getGoBack()->setPosition(ccp(SCREEN.width - 25, SCREEN.height-25));

		_hud->setSkillOne(CCMenuItemImage::create(s_Skill_1_Off, s_Skill_1_On, this, menu_selector(GameLayer::firstSkill)));
		_hud->getSkillOne()->setPosition(ccp(SCREEN.width - (s_betweenButtons ),s_sizeFromSide));

		_hud->setSkillTwo(CCMenuItemImage::create(s_Skill_2_Off, s_Skill_2_On, this, menu_selector(GameLayer::projectileSkill)));
		_hud->getSkillTwo()->setPosition(ccp(SCREEN.width - (s_betweenButtons + s_sidePadding + s_skillButtonSize ), s_sizeFromSide));

		_hud->setSkillThree(CCMenuItemImage::create(s_Skill_3_Off, s_Skill_3_On, this, menu_selector(GameLayer::circleSkill)));
		_hud->getSkillThree()->setPosition(ccp(SCREEN.width - (s_betweenButtons ), s_sizeFromSide + s_horizPadding + s_skillButtonSize));

		_hud->setSkillFour(CCMenuItemImage::create(s_Skill_4_Off, s_Skill_4_On, this, menu_selector(GameLayer::SplitSkill)));
		_hud->getSkillFour()->setPosition(ccp(SCREEN.width - (s_betweenButtons + s_sidePadding + s_skillButtonSize ), s_sizeFromSide + s_horizPadding + s_skillButtonSize));

		CCMenu* pMenu = CCMenu::create(_hud->getSkillOne(), _hud->getSkillTwo(), _hud->getSkillThree(), _hud->getSkillFour(), _hud->getGoBack(), NULL);
		pMenu->setPosition(CCPointZero);

		pMenu->setTag(10);
		_hud->addChild(pMenu, 10);

		for (int i = 0; i < 6; i++)
		{
			CCSprite *_manaOrb = CCSprite::create();
			if (i < _cherry->getManaPool())
			{
				_manaOrb->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_ManaFull));
			} else
			{
				_manaOrb->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_ManaEmpty));
			}
			_manaOrb->setPosition(ccp(SCREEN.width/3.35+ ((s_manaSize * i) + (s_manaSpaceSize*i)), SCREEN.height/11));
			_manaOrb->setTag(i);
			_hud->getManaPool()->addObject(_manaOrb);
			_hud->addChild(_manaOrb, 11);
		}

		for (int i = 0; i <= _cherry->getHitPoints() / 10; i++)
		{
			CCSprite *_hpBlip = CCSprite::create();
			if (i * 10 <= _cherry->getHitPoints())
			{
				_hpBlip->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_HpBlip));
			}
			_hpBlip->setPosition(ccp(SCREEN.width/3.29+ ((s_blipSize * i) + (s_blipSpaceSize*i)) - s_horizPadding, SCREEN.height/4.8));
			_hpBlip->setTag(i);
			_hud->getHpPool()->addObject(_hpBlip);
			_hud->addChild(_hpBlip, 7);
		}

		//		for (int i = 0; i < 6; i++)
		//		{
		//			CCSprite *_playerLife = NULL;
		//			if (i < _cherry->getLife())
		//			{
		//				_playerLife = CCSprite::create("LIFE.png");
		//			}
		//			_playerLife->setPosition(ccp(SCREEN.width/3.35 + ((_lifeSize *i) + (_lifeSpaceSize*i)), SCREEN.height/3.2));
		//			_hud->addChild(_playerLife, 7);
		//		}

		_bInit = true;
		this->initStartCutscene();
	}

}

void GameLayer::updateCutscenes(CCObject* pObject)
{

	if (_sceneOne <= 9)
	{
		_sceneOne++;

		if (_sceneOne == 1)
		{
			LOG("%i",_sceneOne);
			_cherry->walkLeftThenIdle();

		}
		if (_sceneOne == 2)
		{
			LOG("%i",_sceneOne);
			_hud->getGameDialouge()->setVisible(true);
			_hud->cherryTalks(true,1);
			_hud->fenemyTalks(false);

		}
		if (_sceneOne == 3)
		{
			LOG("%i",_sceneOne);
			//_fenemy1->walkLeftThenIdle();

			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(true);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge2));

		}
		if (_sceneOne == 4)
		{
			LOG("%i",_sceneOne);
			_hud->cherryTalks(true,4);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge3));

		}
		if (_sceneOne == 5)
		{
			LOG("%i",_sceneOne);
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(true);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge4));

		}
		if (_sceneOne == 6)
		{
			LOG("%i",_sceneOne);
			_dialougeState = false;
			_enemyBeaten = false;
			_checkPointOne = false;
			_enemyCanMove = true;
			_hud->dialougeModeOff();
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge5));

		}
		if (_sceneOne == 7)
		{
			LOG("%i",_sceneOne); //needs toi be removed
			_hud->dialougeModeOn();
			_hud->getGameDialouge()->setVisible(true);
			_hud->cherryTalks(true,2);
			_hud->fenemyTalks(false);
		}
		if (_sceneOne == 8)
		{
			LOG("%i",_sceneOne);
			_hud->cherryTalks(true,3);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge6));
			_hud->getGameDialouge()->setVisible(true);
		}
		if (_sceneOne == 9)
		{
			LOG("%i",_sceneOne);
			_cutsceneOneDone = true;
			_dialougeState = false;
			_hud->getGameDialouge()->setVisible(false);
			_hud->dialougeModeOff();
		}
		LOG("%i",_sceneOne);
	}
	if (_sceneTwo <= 13 && _cutsceneOneDone && _reachedBoss)
	{
		_sceneTwo++;

		if (_sceneTwo == 1)
		{
			LOG("%i",_sceneTwo);
			_cherry->walkLeftThenIdle();
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(false);
			_stopCamUpdate=true;
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge7));

		}
		if (_sceneTwo == 2)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(true,2);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setVisible(true);

		}
		if (_sceneTwo == 3)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(true,3);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge8));

		}
		if (_sceneTwo == 4)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(true);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge9));

		}
		if (_sceneTwo == 5)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(true,3);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge10));

		}
		if (_sceneTwo == 6)
		{
			LOG("%i",_sceneTwo);
			_dialougeState = false;
			_checkPointOneSceneTwo = false;
			_bossCanMove = true;
			_hud->dialougeModeOff();
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(false);

		}
		if (_sceneTwo == 7)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(true);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge11));
			_hud->dialougeModeOn();
			_hud->getGameDialouge()->setVisible(true);

		}
		if (_sceneTwo == 8)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(true,3);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge12));
		}
		if (_sceneTwo == 9)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(false,2);
			_hud->fenemyTalks(true);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge13));
		}
		if (_sceneTwo == 10)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(true,2);
			_hud->fenemyTalks(false);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge14));
		}
		if (_sceneTwo == 11)
		{
			LOG("%i",_sceneTwo);
			_hud->cherryTalks(false,1);
			_hud->fenemyTalks(true);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Dialouge15));
		}
		if (_sceneTwo == 12)
		{
			LOG("%i",_sceneTwo);
			_cutsceneTwoDone = true;
			_dialougeState = false;
			_hud->getGameDialouge()->setVisible(false);
			_hud->dialougeModeOff();
		}

		if (_sceneTwo == 13)
		{
			LOG("%i",_sceneTwo);
			_cutsceneTwoDone = true;
			_dialougeState = false;
			_hud->getGameDialouge()->setVisible(false);
			_hud->dialougeModeOff();
			this->endGame();
		}
		LOG("%i",_sceneTwo);
	}

}
void GameLayer::demoDone()
{
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
}

void GameLayer::didChangeDirectionTo(SimpleDPad *simpleDPad, CCPoint direction)
{
	if (!_dialougeState)
		_cherry->walkWithDirection(direction);
	else
		_cherry->idle();
}

void GameLayer::isHoldingDirection(SimpleDPad *simpleDPad, CCPoint direction)
{
	if (!_dialougeState)
		_cherry->walkWithDirection(direction);
	else
		_cherry->idle();

}

void GameLayer::simpleDPadTouchEnded(SimpleDPad *simpleDPad)
{
	if (_cherry->getActionState() == kActionStateWalk)
	{
		_cherry->idle();
	}
}

void GameLayer::update(float dt)
{
	_cherry->update(dt);

	this->updatePositions();
	this->updateEnemies(dt);
	this->updateBoss(dt);
	this->setViewpointCenter(_cherry->getPosition());

	this->reorderActors();

	this->updateProjectiles();
	this->updateBossProjectiles();
	this->updateUI();

}

void GameLayer::updateProjectiles()
{
	CCArray *projectilesToDelete = new CCArray;
	CCObject* it = NULL;

	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
	CCARRAY_FOREACH(_projectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
				projectile->getPosition().x - (projectile->getContentSize().width/2),
				projectile->getPosition().y - (projectile->getContentSize().height/2),
				projectile->getContentSize().width,
				projectile->getContentSize().height);

		CCObject *pObject = NULL;
		CCARRAY_FOREACH(_enemies, pObject)
		{
			EnemyFemale *enemy = (EnemyFemale*) pObject;
			if (enemy->getActionState() != kActionStateKnockedOut)
			{
				if (projectileRect.intersectsRect(enemy->getHitbox().actual))
				{
					enemy->hurtWithDamage(_cherry->getProjectileDamage());
					projectile->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
				}
			}

		}
		if (_fenemy1->getActionState() != kActionStateKnockedOut)
		{
			if (projectileRect.intersectsRect(_fenemy1->getHitbox().actual))
			{
				_fenemy1->hurtWithDamage(_cherry->getProjectileDamage());
				projectile->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
			}
		}
		if (_eBoss->getActionState() != kActionStateKnockedOut)
		{
			if (projectileRect.intersectsRect(_eBoss->getHitbox().actual))
			{
				_eBoss->hurtWithDamage(_cherry->getProjectileDamage());
				projectile->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
			}
		}
	}

	// for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
	CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}
void GameLayer::updateBossProjectiles()
{
	CCArray *projectilesToDelete = new CCArray;
	CCObject* it = NULL;

	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
	CCARRAY_FOREACH(_bossProjectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
				projectile->getPosition().x - (projectile->getContentSize().width/2),
				projectile->getPosition().y - (projectile->getContentSize().height/2),
				projectile->getContentSize().width,
				projectile->getContentSize().height);

		if (_cherry->getActionState() != kActionStateKnockedOut)
		{
			if (projectileRect.intersectsRect(_cherry->getHitbox().actual))
			{
				LOG("damage cherry");
				_cherry->hurtWithDamage(_eBoss->getProjectileDamage());
				projectile->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
			}
		}
	}

	// for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
	CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_bossProjectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}

void GameLayer::updatePositions()
{

	//add movement restrictions for different scenes!

	float posX = MIN(_tileMap->getMapSize().width * _tileMap->getTileSize().width - _cherry->getCenterToSides(),
			MAX(_cherry->getCenterToSides()*4, _cherry->getDesiredPosition().x));
	float posY = MIN(3 * _tileMap->getTileSize().height + _cherry->getCenterToBottom(),
			MAX(_cherry->getCenterToBottom()+5, _cherry->getDesiredPosition().y));
	_cherry->setPosition(ccp(posX, posY));

	posX = MIN(_tileMap->getMapSize().width * _tileMap->getTileSize().width - _fenemy1->getCenterToSides(),
			MAX(_fenemy1->getCenterToSides(), _fenemy1->getDesiredPosition().x));
	posY = MIN(3 * _tileMap->getTileSize().height + _fenemy1->getCenterToBottom(),
			MAX(_fenemy1->getCenterToBottom(), _fenemy1->getDesiredPosition().y));
	_fenemy1->setPosition(ccp(posX, posY));

	posX = _tileMap->getMapSize().width * _tileMap->getTileSize().width - _eBoss->getCenterToSides() * 5;

	posY = MIN(3 * _tileMap->getTileSize().height + _eBoss->getCenterToBottom(),
			MAX(_eBoss->getCenterToBottom(), _eBoss->getDesiredPosition().y));
	_eBoss->setPosition(ccp(posX, posY));

	if (_eBoss->getActionState() == kActionStateAttack || _eBoss->getActionState() == kActionStateWalk)
	{
		_eBossWings->setPosition(ccp(posX-9, posY));
	}
	else
	{
		_eBossWings->setPosition(ccp(posX, posY));
	}

	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_enemies, pObject)
	{
		EnemyFemale *enemy = (EnemyFemale*) pObject;
		posX = MIN(_tileMap->getMapSize().width * _tileMap->getTileSize().width - enemy->getCenterToSides(),
				MAX(enemy->getCenterToSides(), enemy->getDesiredPosition().x));
		posY = MIN(3 * _tileMap->getTileSize().height + enemy->getCenterToBottom(),
				MAX(enemy->getCenterToBottom(), enemy->getDesiredPosition().y));
		enemy->setPosition(ccp(posX, posY));
	}
}

void GameLayer::updateUI()
{
	CCObject *item;
	int i = 0;
	CCARRAY_FOREACH(_hud->getManaPool(), item)
	{
		i++;
		CCSprite *s = ((cocos2d::CCSprite*) item);
		if (s != NULL)
		{
			if (i <= _cherry->getManaPool())
			{
				s->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_ManaFull));
			} else
			{
				s->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_ManaEmpty));
			}
		}
		if (_dialougeState)
			s->setVisible(false);
		else
			s->setVisible(true);
	}

	i = 0;
	CCARRAY_FOREACH(_hud->getHpPool(), item)
	{
		i++;
		CCSprite *s = ((cocos2d::CCSprite*) item);
		if (s != NULL)
		{
			if (i * 10 <= _cherry->getHitPoints())
			{
				s->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_HpBlip));
			} else
			{
				s->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_HpBlip));
				s->setOpacity(10);
			}
		}
		if (_dialougeState)
			s->setVisible(false);
		else
			s->setVisible(true);
	}

	if (!_dialougeState)
		_hud->dialougeModeOff();
	if (_dialougeState)
		_hud->dialougeModeOn();

}

void GameLayer::setViewpointCenter(CCPoint position)
{
	if (!_stopCamUpdate)
	{
		int x = MAX(position.x, SCREEN.width / 2);
		int y = MAX(position.y, SCREEN.height / 2);
		x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - SCREEN.width / 2);
		y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - SCREEN.height / 2);
		CCPoint actualPosition = ccp(x, y);

		CCPoint centerOfView = ccp(SCREEN.width / 2, SCREEN.height / 2);
		CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
		this->setPosition(viewPoint);
	}
}

void GameLayer::reorderActors()
{
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_actorsAtlas->getChildren(), pObject)
	{
		ActionSprite *sprite = (ActionSprite*) pObject;
		_actorsAtlas->reorderChild(sprite, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - sprite->getPosition().y);
	}
}
void GameLayer::updateBoss(float dt)
{
	float distanceSQ;
	int randomChoice = 0;
	CCObject *pObject = NULL;
	int bossAttackRange = (!_reachedBoss ? 100000 : 129000 * 4);

	_eBoss->update(dt);
	_eBossWings->update(dt);
	//	LOG("bossAttackRange: %i",bossAttackRange);
	if (_eBoss->getActionState() != kActionStateKnockedOut && !_dialougeState)
	{
		if (CURTIME > _eBoss->getNextDecisionTime())
		{

			distanceSQ = ccpDistanceSQ(_eBoss->getPosition(), _cherry->getPosition());
			//cocos2d::CCLog("distanceSQ: %lf", distanceSQ);
			//LOG("distanceSQ %fl", distanceSQ);
			//3 distance for attacks, code needs to be rewritted for more enemies
			if (distanceSQ <= bossAttackRange)
			{
				_reachedBoss = true;
				if (!_bossTalk)
				{
					initStartCutsceneTwo();
				}
				_eBoss->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.2) * 1000);
				randomChoice = random_range(0, 1);

				if (randomChoice == 0)
				{
					if (_cherry->getPosition().x > _eBoss->getPosition().x)
					{
						_eBoss->setScaleX(1.0);
					} else
					{
						_eBoss->setScaleX(-1.0);
					}

					//4
					_eBoss->setNextDecisionTime(_eBoss->getNextDecisionTime() + frandom_range(0.1,0.2) * 2000);

					randomChoice = random_range(0, 8);

					if (randomChoice == 1)
					{
						_eBoss->attack();

						if (_eBoss->getActionState() == kActionStateAttack)
						{
							if (fabsf(_cherry->getPosition().y - _eBoss->getPosition().y) < 20)
							{
								if (_cherry->getHitbox().actual.intersectsRect(_eBoss->getAttackBox().actual) && _hud->getChildByTag(100) == NULL)
								{
									_cherry->hurtWithDamage(_eBoss->getDamage());

								}
							}
						}
					} else if (randomChoice == 2)
					{
						this->bossSecondSkill();
					} else if (randomChoice == 3)
					{
						this->bossFirstSkill();
					} else
					{
						CCPoint moveDirection = ccpNormalize(ccpSub(_cherry->getPosition(), _eBoss->getPosition()));
						_eBoss->walkWithDirection(moveDirection);
						//_eBossWings->walkWithDirection(moveDirection);
					}
				} else
				{
					_eBoss->idle();
				}
			} else if (distanceSQ <= SCREEN.width * SCREEN.width)
			{
				//5
				_eBoss->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5) * 1000);
				_eBoss->idle();
			}
		}
	}
	if (_eBoss->getActionState() == kActionStateKnockedOut)
	{
		_bossDead = true;

	}
	if (_bossDead && !_checkPointOneSceneTwo)
	{
		_dialougeState = true;
		_checkPointOneSceneTwo = true;
		_hud->dialougeModeOn();
		this->updateUI();
		_eBoss->knockout();
		_eBossWings->knockout();
	}

}
void GameLayer::bossFirstSkill()
{
	_eBoss->attack();

	CCSprite *projectile = CCSprite::create(s_BossSmallCoin);
	projectile->setPosition(ccp(_eBoss->getPositionX(), _eBoss->getPositionY()));

	// Ok to add now - we've double checked position
	this->addChild(projectile);

	// Determine where we wish to shoot the projectile to
	int realX = _eBoss->getPositionX() - SCREEN.width;
	int realY = _eBoss->getPositionY();

	CCPoint realDest = ccp(realX, realY);

	// Determine the length of how far we're shooting
	int offRealX = realX - _eBoss->getPositionX();
	int offRealY = realY - _eBoss->getPositionY();

	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));

	float velocity = 480 / 1;	// 480pixels/1sec

	float realMoveDuration = length / velocity;

	CCDelayTime* delayAction = CCDelayTime::create(2.0f);

	// Move projectile to actual endpoint
	projectile->runAction(CCSequence::create(delayAction, CCMoveTo::create(realMoveDuration, realDest), CCFadeOut::create(0.5), CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));

	// Add to projectiles array
	projectile->setTag(1);

	_bossProjectiles->addObject(projectile);

	_bossEmitter = new CCParticleSystemQuad();

	std::string filename = s_SpookyCoins;
	_bossEmitter->initWithFile(filename.c_str());

	// texture
	_bossEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_BossSmallCoin));

	// additive
	_bossEmitter->setBlendAdditive(true);
	_bossEmitter->setDuration(0.2f);

	_bossEmitter->setPosition(_eBoss->getPositionX(), _eBoss->getPositionY());
	_bossEmitter->setAutoRemoveOnFinish(true);

	this->addChild(_bossEmitter, 20);

}
void GameLayer::bossSecondSkill()
{
	_eBoss->projectileAttack();
	CCSprite *projectile = CCSprite::create(s_BossBigCoin);
	projectile->setPosition(ccp(_eBoss->getPositionX(), SCREEN.height/2));

	// Ok to add now - we've double checked position
	this->addChild(projectile);

	// Determine where we wish to shoot the projectile to
	int realX = _cherry->getPositionX();
	int realY = _cherry->getPositionY();

	CCPoint realDest = ccp(realX, realY);

	// Determine the length of how far we're shooting
	int offRealX = realX - _eBoss->getPositionX();
	int offRealY = realY - _eBoss->getPositionY();

	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));

	float velocity = 240 / 1;	// 480pixels/1sec

	float realMoveDuration = length / velocity;

	CCDelayTime* delayAction = CCDelayTime::create(2.0f);

	// Move projectile to actual endpoint
	projectile->runAction(CCSequence::create(delayAction, CCMoveTo::create(realMoveDuration, realDest), CCFadeOut::create(0.5), CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));

	// Add to projectiles array
	projectile->setTag(1);

	_bossProjectiles->addObject(projectile);

	_bossEmitter = new CCParticleSystemQuad();

	std::string filename = s_SpookyCoins;
	_bossEmitter->initWithFile(filename.c_str());

	// texture
	_bossEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_BossSmallCoin));

	// additive
	_bossEmitter->setBlendAdditive(true);
	_bossEmitter->setDuration(0.5f);

	_bossEmitter->setPosition(projectile->getPositionX(), projectile->getPositionY());
	_bossEmitter->setAutoRemoveOnFinish(true);

	this->addChild(_bossEmitter, 20);
//	}
}

void GameLayer::updateEnemies(float dt)
{
	float distanceSQ;
	int randomChoice = 0;
	CCObject *pObject = NULL;
//
	if (!_enemyBeaten && _enemyCanMove)
	{
		_fenemy1->update(dt);
		if (_fenemy1->getActionState() != kActionStateKnockedOut)
		{
			if (CURTIME > _fenemy1->getNextDecisionTime())
			{
				distanceSQ = ccpDistanceSQ(_fenemy1->getPosition(), _cherry->getPosition());
				//cocos2d::CCLog("distanceSQ: %lf", distanceSQ);

				//3 distance for attacks, code needs to be rewritted for more enemies
				if (distanceSQ <= 700)
				{
					_fenemy1->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5) * 1000);
					randomChoice = random_range(0, 1);

					if (randomChoice == 0)
					{
						if (_cherry->getPosition().x > _fenemy1->getPosition().x)
						{
							_fenemy1->setScaleX(1.0);
						} else
						{
							_fenemy1->setScaleX(-1.0);
						}

						//4
						_fenemy1->setNextDecisionTime(_fenemy1->getNextDecisionTime() + frandom_range(0.1,0.5) * 2000);
						_fenemy1->attack();
						if (_fenemy1->getActionState() == kActionStateAttack)
						{
							if (fabsf(_cherry->getPosition().y - _fenemy1->getPosition().y) < 20)
							{
								if (_cherry->getHitbox().actual.intersectsRect(_fenemy1->getAttackBox().actual) && _hud->getChildByTag(50) == NULL)
								{
									_cherry->hurtWithDamage(_fenemy1->getDamage());

								}
							}
						}
					} else
					{
						_fenemy1->idle();
					}
				} else if (distanceSQ <= SCREEN.width * SCREEN.width)
				{
					//5
					_fenemy1->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5) * 1000);
					randomChoice = random_range(0, 2);
					if (randomChoice == 0)
					{
						CCPoint moveDirection = ccpNormalize(ccpSub(_cherry->getPosition(), _fenemy1->getPosition()));
						_fenemy1->walkWithDirection(moveDirection);
					}
					else
					{
						_fenemy1->idle();
					}
				}
			}
		}
		if (_fenemy1->getActionState() == kActionStateKnockedOut)
		{
			_enemyBeaten = true;
		}
	}

	if (_enemyBeaten && !_cutsceneOneDone && !_checkPointOne)
	{
		_dialougeState = true;
		_checkPointOne = true;
		_hud->dialougeModeOn();
		this->updateUI();
	}

//	LOG("single enemy update");
	if (!_dialougeState)
	{
		CCARRAY_FOREACH(_enemies, pObject)
		{
			EnemyFemale *enemy = (EnemyFemale*) pObject;
			enemy->update(dt);
			if (enemy->getActionState() != kActionStateKnockedOut)
			{

				//2
				if (CURTIME > enemy->getNextDecisionTime())
				{
					distanceSQ = ccpDistanceSQ(enemy->getPosition(), _cherry->getPosition());
					//cocos2d::CCLog("distanceSQ: %lf", distanceSQ);

					//3 distance for attacks, code needs to be rewritted for more enemies
					if (distanceSQ <= 7000)
					{
						enemy->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5) * 1000);
						randomChoice = random_range(0, 1);

						if (randomChoice == 0)
						{
							if (_cherry->getPosition().x > enemy->getPosition().x)
							{
								enemy->setScaleX(1.0);
							} else
							{
								enemy->setScaleX(-1.0);
							}

							//4
							enemy->setNextDecisionTime(enemy->getNextDecisionTime() + frandom_range(0.1, 0.5) * 2000);
							enemy->attack();
							if (enemy->getActionState() == kActionStateAttack)
							{
								if (fabsf(_cherry->getPosition().y - enemy->getPosition().y) < 20)
								{
									if (_cherry->getHitbox().actual.intersectsRect(enemy->getAttackBox().actual) && _hud->getChildByTag(50) == NULL)
									{
										_cherry->hurtWithDamage(enemy->getDamage());

										//end game
										if (_cherry->getActionState() == kActionStateKnockedOut && _hud->getChildByTag(100) == NULL)
										{
											this->endGame();
											LOG("END GAME CHECK 1");
										}
									}
								}
							}
						} else
						{
							enemy->idle();
						}
					} else if (distanceSQ <= SCREEN.width * SCREEN.width)
					{
						//5
						enemy->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5)* 1000);
						randomChoice = random_range(0, 2)
						;
						if (randomChoice == 0)
						{
							CCPoint moveDirection = ccpNormalize(ccpSub(_cherry->getPosition(), enemy->getPosition()));
							enemy->walkWithDirection(moveDirection);
						}
						else
						{
							enemy->idle();
						}
					}
				}
			}
		}
	}
//	LOG("_enemies update");

//end game checker here
	if (_cherry->getActionState() == kActionStateKnockedOut && _hud->getChildByTag(100) == NULL)
	{
		LOG("END GAME CHECK 2");
		this->endGame();
	}

	this->initSkillBar();
}

void GameLayer::firstSkill(CCObject* pObject)
{
	if (_cherry->getActionState() != kActionStateKnockedOut)
	{
		_cherry->attack();

		int x = (_cherry->getScaleX() == -1 ? (_cherry->getAttackBox().actual.origin.x - 20) : (_cherry->getAttackBox().actual.origin.x + 50));
		int y = _cherry->getAttackBox().actual.origin.y;

		if (_cherry->getActionState() == kActionStateAttack)
		{
			if (!_cherry->getAttackDone())
			{
				_cherryEmitter = CCParticleExplosion::create();
				_cherryEmitter->setPosVar(CCPointZero);

				//_cherryEmitter->initWithTotalParticles(200);

				_cherryEmitter->setLifeVar(0);
				_cherryEmitter->setLife(0.7f);
				_cherryEmitter->setEmissionRate(10000);

				_cherryEmitter->setGravity(ccp(0,150));

				_cherryEmitter->setStartSpin(10.0f);
				_cherryEmitter->setStartSpinVar(2.0f);
				_cherryEmitter->setEndSpin(30.0f);
				_cherryEmitter->setEndSpinVar(5.0f);

				_cherryEmitter->setDuration(0.5f);

				_cherryEmitter->setStartSize(0.5f);
				_cherryEmitter->setStartSizeVar(0.5f);
				_cherryEmitter->setEndSize(30.0f);
				_cherryEmitter->setEndSizeVar(5.0f);

				// texture
				_cherryEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

				// additive
				_cherryEmitter->setBlendAdditive(true);

				_cherryEmitter->setPosition(x, y);
				_cherryEmitter->setAutoRemoveOnFinish(true);

				this->addChild(_cherryEmitter, 20);
			}

			CCObject *pObject = NULL;
			CCARRAY_FOREACH(_enemies, pObject)
			{
				EnemyFemale *enemy = (EnemyFemale*) pObject;
				if (enemy->getActionState() != kActionStateKnockedOut)
				{
					if (fabsf(_cherry->getPosition().y - enemy->getPosition().y) < 20)
					{
						if (_cherry->getAttackBox().actual.intersectsRect(enemy->getHitbox().actual))
						{
							enemy->hurtWithDamage(_cherry->getDamage());
							_cherry->setManaPool(_cherry->getManaPool() + 0.5f);
							if (_cherry->getManaPool() >= 6)
								_cherry->setManaPool(6);
						}
					}
				}
			}
			if (_fenemy1->getActionState() != kActionStateKnockedOut)
			{
				if (_cherry->getAttackBox().actual.intersectsRect(_fenemy1->getHitbox().actual))
				{
					_fenemy1->hurtWithDamage(_cherry->getDamage());
				}
			}
			if (_eBoss->getActionState() != kActionStateKnockedOut)
			{
				if (_cherry->getAttackBox().actual.intersectsRect(_eBoss->getHitbox().actual))
				{
					_eBoss->hurtWithDamage(_cherry->getDamage());
				}
			}
		}
	}
}

void GameLayer::SplitSkill(CCObject* pObject)
{
	if (_cherry->getManaPool() >= 4 && _cherry->getActionState() != kActionStateKnockedOut)
	{
		_cherry->setManaPool(_cherry->getManaPool() - 4);
		_cherry->circleAttack();

		CCSprite *_slashCut = CCSprite::create("streak.png");
		_slashCut->setPosition(ccp(_cherry->getPositionX(), SCREEN.height/6));

		_slashCut->runAction(CCSequence::create(CCScaleTo::create(1, 40, 1), CCFadeOut::create(1.5), CCRemoveSelf::create(true), CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
		_slashCut->setTag(3);
		CCSprite *_tempBg = CCSprite::create("tempTest.png");
		_tempBg->setPosition(ccp(_cherry->getPositionX()-100, _cherry->getPositionY()-100));
		_tempBg->setTag(3);
		_tempBg->runAction(CCSequence::create(CCScaleTo::create(0.1, 400, 400), CCFadeOut::create(2.5), CCRemoveSelf::create(true), CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
		this->addChild(_tempBg, 20);
		this->addChild(_slashCut, 21);

		CCObject *pObject = NULL;
		CCARRAY_FOREACH(_enemies, pObject)
		{
			EnemyFemale *enemy = (EnemyFemale*) pObject;
			if (enemy->getActionState() != kActionStateKnockedOut)
			{
				if (_cherry->getSplitAttackBox().actual.intersectsRect(enemy->getHitbox().actual))
				{
					enemy->splitEnemy();
				}
			}
		}
		if (_fenemy1->getActionState() != kActionStateKnockedOut)
		{
			if (_cherry->getSplitAttackBox().actual.intersectsRect(_fenemy1->getHitbox().actual))
			{
				_fenemy1->splitEnemy();
			}
		}
		if (_eBoss->getActionState() != kActionStateKnockedOut)
		{
			if (_eBoss->getSplitAttackBox().actual.intersectsRect(_eBoss->getHitbox().actual))
			{
				_eBoss->hurtWithDamage(_cherry->getSplitDamage());
			}
		}

	}

}

void GameLayer::projectileSkill(CCObject* pObject)
{
	if (_cherry->getManaPool() >= 2 && _cherry->getActionState() != kActionStateKnockedOut)
	{
		_cherry->setManaPool(_cherry->getManaPool() - 2);
		_cherry->projectileAttack();

		CCSprite *projectile = CCSprite::create(s_GrayscaleStars);
		projectile->setPosition(ccp(_cherry->getPositionX(), _cherry->getPositionY()));

		// Ok to add now - we've double checked position
		this->addChild(projectile);

		// Determine where we wish to shoot the projectile to
		int realX = _cherry->getPositionX() + (_cherry->getScaleX() == -1 ? (-SCREEN.width) : SCREEN.width);
		int realY = _cherry->getPositionY();

		CCPoint realDest = ccp(realX, realY);

		// Determine the length of how far we're shooting
		int offRealX = realX - _cherry->getPositionX();
		int offRealY = realY - _cherry->getPositionY();

		float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));

		float velocity = 480 / 1;	// 480pixels/1sec

		float realMoveDuration = length / velocity;

		// Move projectile to actual endpoint
		projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), CCFadeOut::create(0.5), CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));

		// Add to projectiles array
		projectile->setTag(2);

		_projectiles->addObject(projectile);

		_cherryEmitter = new CCParticleSystemQuad();
		std::string filename = "Particles/Phoenix.plist";
		_cherryEmitter->initWithFile(filename.c_str());

		// texture
		_cherryEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

		// additive
		_cherryEmitter->setBlendAdditive(true);
		_cherryEmitter->setDuration(1.2f);

		_cherryEmitter->setPosition(_cherry->getScaleX() == -1 ? (projectile->getPositionX() - 20) : (projectile->getPositionX() + 50), projectile->getPositionY());
		_cherryEmitter->setAutoRemoveOnFinish(true);

		this->addChild(_cherryEmitter, 20);
		_cherryEmitter->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), NULL));
	}

}

void GameLayer::circleSkill(CCObject* pObject)
{
	if (_cherry->getManaPool() >= 3 && _cherry->getActionState() != kActionStateKnockedOut)
	{
		_cherry->setManaPool(_cherry->getManaPool() - 3);
		//		updateUI();
		_cherry->circleAttack();

		int x = _cherry->getPosition().x;
		int y = _cherry->getPosition().y;

		_cherryEmitter = CCParticleFlower::create();

		_cherryEmitter->setPosVar(CCPointZero);

		//_cherryEmitter->initWithTotalParticles(300);

		_cherryEmitter->setLifeVar(0);
		_cherryEmitter->setLife(0.5f);
		_cherryEmitter->setSpeed(400);
		_cherryEmitter->setSpeedVar(20);
		_cherryEmitter->setEmissionRate(10000);

		_cherryEmitter->setStartSpin(10.0f);
		_cherryEmitter->setStartSpinVar(2.0f);
		_cherryEmitter->setEndSpin(30.0f);
		_cherryEmitter->setEndSpinVar(5.0f);

		_cherryEmitter->setDuration(0.5f);

		_cherryEmitter->setStartSize(4.0f);
		_cherryEmitter->setStartSizeVar(2.0f);
		_cherryEmitter->setEndSize(30.0f);
		_cherryEmitter->setEndSizeVar(5.0f);

		// texture
		_cherryEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

		// additive
		_cherryEmitter->setBlendAdditive(true);

		_cherryEmitter->setPosition(x, y);
		_cherryEmitter->setAutoRemoveOnFinish(true);

		this->addChild(_cherryEmitter, 20);

		if (_cherry->getActionState() == kActionStateAttack)
		{
			CCObject *pObject = NULL;
			CCARRAY_FOREACH(_enemies, pObject)
			{
				EnemyFemale *enemy = (EnemyFemale*) pObject;
				if (enemy->getActionState() != kActionStateKnockedOut)
				{
					if (_cherry->getCircleAttackBox().actual.intersectsRect(enemy->getHitbox().actual))
					{
						enemy->hurtWithDamage(_cherry->getCircleDamage());
					}
				}
			}
			if (_fenemy1->getActionState() != kActionStateKnockedOut)
			{
				if (_cherry->getCircleAttackBox().actual.intersectsRect(_fenemy1->getHitbox().actual))
				{
					_fenemy1->hurtWithDamage(_cherry->getCircleDamage());
				}
			}
			if (_eBoss->getActionState() != kActionStateKnockedOut)
			{
				if (_eBoss->getCircleAttackBox().actual.intersectsRect(_eBoss->getHitbox().actual))
				{
					_eBoss->hurtWithDamage(_cherry->getCircleDamage());
				}
			}
		}
	}

}
void GameLayer::draw()
{
	if (showHitBox)
	{
		//normal attack
//				CCPoint p1 = ccp(_cherry->getAttackBox().actual.origin.x,_cherry->getAttackBox().actual.origin.y);
//				CCPoint p2 = ccp(_cherry->getAttackBox().actual.origin.x + _cherry->getAttackBox().actual.size.width,_cherry->getAttackBox().actual.origin.y + _cherry->getAttackBox().actual.size.height);
		CCPoint p1 = ccp(_cherry->getHitbox().actual.origin.x,_cherry->getHitbox().actual.origin.y);
		CCPoint p2 = ccp(_cherry->getHitbox().actual.origin.x + _cherry->getHitbox().actual.size.width,_cherry->getHitbox().actual.origin.y + _cherry->getHitbox().actual.size.height);
		//circle
		//		CCPoint p1 = ccp(_cherry->getCircleAttackBox().actual.origin.x,_cherry->getCircleAttackBox().actual.origin.y);
		//		CCPoint p2 = ccp(_cherry->getCircleAttackBox().actual.origin.x + _cherry->getCircleAttackBox().actual.size.width,_cherry->getCircleAttackBox().actual.origin.y + _cherry->getCircleAttackBox().actual.size.height);
		//
		//		CCPoint p1 = ccp(_cherry->getSplitAttackBox().actual.origin.x,_cherry->getSplitAttackBox().actual.origin.y);
		//		CCPoint p2 = ccp(_cherry->getSplitAttackBox().actual.origin.x + _cherry->getSplitAttackBox().actual.size.width,_cherry->getSplitAttackBox().actual.origin.y + _cherry->getSplitAttackBox().actual.size.height);
		//
				ccDrawColor4B(0, 0, 0, 255);
				ccDrawRect(p1, p2);

		//
		//		CCObject *pObject = NULL;
		//		CCARRAY_FOREACH(_enemies, pObject)
		//		{
		//			EnemyFemale *enemy = (EnemyFemale*) pObject;
		//			CCPoint p1 = ccp(enemy->getHitbox().actual.origin.x,enemy->getHitbox().actual.origin.y);
		//			CCPoint p2 = ccp(enemy->getHitbox().actual.origin.x + enemy->getHitbox().actual.size.width,enemy->getHitbox().actual.origin.y + enemy->getHitbox().actual.size.width);
		//
		//			ccDrawColor4B(0, 0, 0, 255);
		//			ccDrawRect(p1, p2);
		//
		//		}
		//		CCPoint p1 = ccp(_cherry->getPositionX(),_cherry->getPositionY());
		//		CCPoint p2 = ccp(_cherry->getPositionX() + 100,_cherry->getPositionY());
		//		glLineWidth(20.0f);
		//		ccDrawColor4B(255, 255, 255, 255);
		//		ccDrawLine(p1, p2);

			}
		}

void GameLayer::objectRemoval(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *) sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 1)  // boss projectile
	{
		_bossProjectiles->removeObject(sprite);

//		if (_cherryEmitter->isActive())
//			this->removeChild(_cherryEmitter, true);

//		_cherryEmitter = new CCParticleSystemQuad();
//		std::string filename = s_Phoenix;
//		_cherryEmitter->initWithFile(filename.c_str());
//
//		// texture
//		_cherryEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_BossSmallCoin));
//
//		// additive
//		_cherryEmitter->setBlendAdditive(true);
//		_cherryEmitter->setLife(0.1f);
//		_cherryEmitter->setLifeVar(0.1f);
//		_cherryEmitter->setDuration(0.1f);
//
//		_cherryEmitter->setPosition(sprite->getPositionX(), sprite->getPositionY());
//		_cherryEmitter->setAutoRemoveOnFinish(true);
//
//		this->addChild(_cherryEmitter, 20);
//		cocos2d::CCLog("_bossProjectiles->removeObject(sprite);");
	} else if (sprite->getTag() == 2) // projectile
	{
		_projectiles->removeObject(sprite);

		if (_cherryEmitter->isActive())
			this->removeChild(_cherryEmitter, true);

		_cherryEmitter = new CCParticleSystemQuad();
		std::string filename = "Particles/Phoenix.plist";
		_cherryEmitter->initWithFile(filename.c_str());

		// texture
		_cherryEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

		// additive
		_cherryEmitter->setBlendAdditive(true);
		_cherryEmitter->setLife(0.1f);
		_cherryEmitter->setLifeVar(0.1f);
		_cherryEmitter->setDuration(0.1f);

		_cherryEmitter->setPosition(sprite->getPositionX(), sprite->getPositionY());
		_cherryEmitter->setAutoRemoveOnFinish(true);

		this->addChild(_cherryEmitter, 20);

		cocos2d::CCLog("_projectiles->removeObject(sprite);");
	} else if (sprite->getTag() == 3)
	{
		this->removeChild(sprite);
	}
}

void GameLayer::endGame()
{
	CCMenuItemImage *goBack = CCMenuItemImage::create(s_RetryOff, s_RetryOn, this, menu_selector(GameLayer::restartGame));

	CCSprite *overLayer = CCSprite::create(s_EndOverlay);
	CCSprite *gameOverLogo = CCSprite::create(s_CherryLogoEnd);
	CCSprite *gameOverCredits = CCSprite::create(s_EndCredits);

	goBack->setPosition(ccp(SCREEN.width/2, SCREEN.height/7.5));

	overLayer->setPosition(ccp(SCREEN.width/2, SCREEN.height/2));

	gameOverLogo->setPosition(ccp(SCREEN.width/2, SCREEN.height/1.3));
	gameOverCredits->setPosition(ccp(SCREEN.width/2, SCREEN.height/3.7));

	CCMenu* pMenu = CCMenu::create(goBack, NULL);
	pMenu->setPosition(CCPointZero);

	pMenu->setTag(100);
	_hud->addChild(overLayer, 100);

	_hud->addChild(pMenu, 102);
	_hud->addChild(gameOverLogo, 101);
	_hud->addChild(gameOverCredits, 101);
}

void GameLayer::restartGame(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameScene::create());
}

void GameLayer::mainMenu(CCObject* pObject)
{
	CCDirector::sharedDirector()->pushScene(MenuScene::scene());
}
void GameLayer::keyBackClicked(void)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
