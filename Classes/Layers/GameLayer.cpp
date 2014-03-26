#include "../Layers/GameLayer.h"
#include "../GameObjects/EnemyFemale.h"
#include "../Scenes/GameScene.h"
#include "SimpleAudioEngine.h"
#include "../Defines.h"

using namespace cocos2d;
bool showHitBox = true;

GameLayer::GameLayer(void) :
		_projectiles(NULL)
{
	_tileMap = NULL;
	_cherry = NULL;
	_fenemy1 = NULL;
	_enemies = NULL;
	_bInit = false;
	_dialougeState = false;
	_enemyBeaten = true;

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
		this->initCherry();
		LOG("initCherry");
		this->initEnemies();
		LOG("initEnemies");

		this->scheduleUpdate();
		LOG("scheduleUpdate");
		this->setKeypadEnabled(true);

		m_emitter = new CCParticleSystemQuad();
		CC_BREAK_IF(!m_emitter);

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
	_checkPointOne = true;
	_checkPointTwo = true;
	_dialougeState = true;
	_enemyBeaten = false;

	if (!_dialougeState)
	_hud->dialougeModeOff();
	if (_dialougeState)
	_hud->dialougeModeOn();

	_hud->getGameDialouge()->setVisible(false);
	_hud->getGameDialouge()->setPosition(ccp(SCREEN.width/2, SCREEN.height/5));
}

void GameLayer::initCherry()
{
	_cherry = Cherry::create();
	_actorsAtlas->addChild(_cherry);
	_cherry->setPosition(ccp(_cherry->getCenterToSides(), 80));
	_cherry->setDesiredPosition(_cherry->getPosition());
	_cherry->idle();
	LOG("Cherry done");

	_fenemy1 = EnemyFemale::create();
	_actorsAtlas->addChild(_fenemy1);
	_fenemy1->setPosition(ccp(SCREEN.width, 80));
	_fenemy1->setDesiredPosition(_fenemy1->getPosition());
	_fenemy1->setScaleX(-1);
	_fenemy1->idle();
	LOG("EnemyFemale done");
}
void GameLayer::initEnemies()
{
	int enemyCount = 25;
	this->setEnemies(CCArray::createWithCapacity(enemyCount));
	LOG("Started enemies init");
	for (int i = 0; i < enemyCount; i++)
	{
		LOG("%i",i);
		EnemyFemale *enemy = EnemyFemale::create();
		_actorsAtlas->addChild(enemy);
		_enemies->addObject(enemy);

		int minX = SCREEN.width + enemy->getCenterToSides()*3;
		int maxX = _tileMap->getMapSize().width * _tileMap->getTileSize().width - enemy->getCenterToSides();
		int minY = enemy->getCenterToBottom();
		int maxY = 3 * _tileMap->getTileSize().height + enemy->getCenterToBottom();
		enemy->setScaleX(-1);
		enemy->setPosition(ccp(random_range(minX, maxX), random_range(minY, maxY)));
		enemy->setDesiredPosition(enemy->getPosition());
		enemy->idle();
		LOG("%i",i);
	}
	LOG("done enemies init");

}
void GameLayer::initSkillBar()
{
	if (_hud != NULL && _hud->getChildByTag(10) == NULL && _bInit == false)
	{
		LOG("initSkillBar");
		_hud->setGoBack(CCMenuItemImage::create(s_PauseOff, s_PauseOn, this, menu_selector(GameLayer::mainMenu)));

		_hud->getGoBack()->setPosition(ccp(SCREEN.width - 20, SCREEN.height-20));

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

void GameLayer::updateCutsceneOne()
{

	if (_checkPointOne && _sceneOne <= 10)
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

		}
		if (_sceneOne == 3)
		{
			LOG("%i",_sceneOne);
			_fenemy1->walkLeftThenIdle();

		}
		if (_sceneOne == 4)
		{
			LOG("%i",_sceneOne);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox002.png"));

		}
		if (_sceneOne == 5)
		{
			LOG("%i",_sceneOne);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox003.png"));

		}
		if (_sceneOne == 6)
		{
			LOG("%i",_sceneOne);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox004.png"));

		}
		if (_sceneOne == 7)
		{
			LOG("%i",_sceneOne);
			_dialougeState = false;
			_hud->dialougeModeOff();

		}
		if (_sceneOne == 8)
		{
			LOG("%i",_sceneOne);
			_hud->dialougeModeOn();
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox005.png"));
		}
		if (_sceneOne == 9)
		{
			LOG("%i",_sceneOne);
			_hud->getGameDialouge()->setTexture(CCTextureCache::sharedTextureCache()->addImage("dialougebox006.png"));
		}
		if (_sceneOne == 10)
		{
			LOG("%i",_sceneOne);
			_checkPointOne = false;
			_dialougeState = false;
			_hud->getGameDialouge()->setVisible(false);
			_hud->dialougeModeOff();
		}
		LOG("%i",_sceneOne);
	}

}

void GameLayer::updateCutsceneTwo()
{

}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (_checkPointOne && _dialougeState)
		updateCutsceneOne();
	if (_checkPointTwo && !_checkPointOne && _dialougeState)
		updateCutsceneTwo();
}

//
void GameLayer::didChangeDirectionTo(SimpleDPad *simpleDPad, CCPoint direction)
{
	if (!_dialougeState)
		_cherry->walkWithDirection(direction);
}

void GameLayer::isHoldingDirection(SimpleDPad *simpleDPad, CCPoint direction)
{
	if (!_dialougeState)
		_cherry->walkWithDirection(direction);

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
	this->setViewpointCenter(_cherry->getPosition());

	this->updateProjectiles();
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
					cocos2d::CCLog("enemy->hurtWithDamage(_cherry->getDamage());");
				}
			}

		}
		if (_fenemy1->getActionState() != kActionStateKnockedOut)
		{
			if (projectileRect.intersectsRect(_fenemy1->getHitbox().actual))
			{
				_fenemy1->hurtWithDamage(_cherry->getProjectileDamage());
				projectile->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));
				cocos2d::CCLog("enemy->hurtWithDamage(_cherry->getDamage());");
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

void GameLayer::updatePositions()
{
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

}

void GameLayer::setViewpointCenter(CCPoint position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
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

void GameLayer::updateEnemies(float dt)
{
	float distanceSQ;
	int randomChoice = 0;
	CCObject *pObject = NULL;
//
	if (_enemyBeaten)
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
						_fenemy1->setNextDecisionTime(_fenemy1->getNextDecisionTime() + frandom_range(0.1, 0.2) * 2000);
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
							enemy->setNextDecisionTime(enemy->getNextDecisionTime() + frandom_range(0.1, 0.2) * 2000);
							enemy->attack();
							if (enemy->getActionState() == kActionStateAttack)
							{
								if (fabsf(_cherry->getPosition().y - enemy->getPosition().y) < 20)
								{
									if (_cherry->getHitbox().actual.intersectsRect(enemy->getAttackBox().actual) && _hud->getChildByTag(50) == NULL)
									{
										_cherry->hurtWithDamage(enemy->getDamage());

										//end game
										if (_cherry->getActionState() == kActionStateKnockedOut)
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
	if (_cherry->getActionState() == kActionStateKnockedOut && _hud->getChildByTag(50) == NULL)
	{
		LOG("END GAME CHECK 2");
		this->endGame();
	}

	this->initSkillBar();
}

void GameLayer::firstSkill()
{

	_cherry->attack();

	int x = (_cherry->getScaleX() == -1 ? (_cherry->getAttackBox().actual.origin.x - 20) : (_cherry->getAttackBox().actual.origin.x + 50));
	int y = _cherry->getAttackBox().actual.origin.y;

	m_emitter = CCParticleExplosion::create();

	m_emitter->setPosVar(CCPointZero);

	m_emitter->initWithTotalParticles(200);

	m_emitter->setLifeVar(0);
	m_emitter->setLife(0.7f);
	m_emitter->setEmissionRate(10000);

	m_emitter->setGravity(ccp(0,150));

	m_emitter->setStartSpin(10.0f);
	m_emitter->setStartSpinVar(2.0f);
	m_emitter->setEndSpin(30.0f);
	m_emitter->setEndSpinVar(5.0f);

	m_emitter->setDuration(0.5f);

	m_emitter->setStartSize(0.5f);
	m_emitter->setStartSizeVar(0.5f);
	m_emitter->setEndSize(30.0f);
	m_emitter->setEndSizeVar(5.0f);

	// texture
	m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

	// additive
	m_emitter->setBlendAdditive(true);

	m_emitter->setPosition(x, y);
	m_emitter->setAutoRemoveOnFinish(true);

	this->addChild(m_emitter, 20);

	if (_cherry->getActionState() == kActionStateAttack)
	{
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
	}

}

void GameLayer::SplitSkill()
{
	if (_cherry->getManaPool() >= 4)
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

	}

}

void GameLayer::projectileSkill()
{
	if (_cherry->getManaPool() >= 2)
	{
		_cherry->setManaPool(_cherry->getManaPool() - 2);
		_cherry->projectileAttack();

		CCSprite *projectile = CCSprite::create("stars-grayscale.png");
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
		projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), CCBlink::create(1.0, 3.0), CCCallFuncN::create(this, callfuncN_selector(GameLayer::objectRemoval)), NULL));

		// Add to projectiles array
		projectile->setTag(2);

		_projectiles->addObject(projectile);

		m_emitter = new CCParticleSystemQuad();
		std::string filename = "Particles/Phoenix.plist";
		m_emitter->initWithFile(filename.c_str());

		// texture
		m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

		// additive
		m_emitter->setBlendAdditive(true);
		m_emitter->setDuration(1.2f);

		m_emitter->setPosition(_cherry->getScaleX() == -1 ? (projectile->getPositionX() - 20) : (projectile->getPositionX() + 50), projectile->getPositionY());
		m_emitter->setAutoRemoveOnFinish(true);

		this->addChild(m_emitter, 20);
		m_emitter->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), NULL));
	}

}

void GameLayer::circleSkill()
{
	if (_cherry->getManaPool() >= 3)
	{
		_cherry->setManaPool(_cherry->getManaPool() - 3);
//		updateUI();
		_cherry->circleAttack();

		int x = _cherry->getPosition().x;
		int y = _cherry->getPosition().y;

		m_emitter = CCParticleFlower::create();

		m_emitter->setPosVar(CCPointZero);

		m_emitter->initWithTotalParticles(300);

		m_emitter->setLifeVar(0);
		m_emitter->setLife(0.5f);
		m_emitter->setSpeed(400);
		m_emitter->setSpeedVar(20);
		m_emitter->setEmissionRate(10000);

		m_emitter->setStartSpin(10.0f);
		m_emitter->setStartSpinVar(2.0f);
		m_emitter->setEndSpin(30.0f);
		m_emitter->setEndSpinVar(5.0f);

		m_emitter->setDuration(0.5f);

		m_emitter->setStartSize(4.0f);
		m_emitter->setStartSizeVar(2.0f);
		m_emitter->setEndSize(30.0f);
		m_emitter->setEndSizeVar(5.0f);

// texture
		m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

// additive
		m_emitter->setBlendAdditive(true);

		m_emitter->setPosition(x, y);
		m_emitter->setAutoRemoveOnFinish(true);

		this->addChild(m_emitter, 20);

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
		}
	}

}
void GameLayer::draw()
{
	if (showHitBox)
	{
		//normal attack
		//		CCPoint p1 = ccp(_cherry->getAttackBox().actual.origin.x,_cherry->getAttackBox().actual.origin.y);
		//		CCPoint p2 = ccp(_cherry->getAttackBox().actual.origin.x + _cherry->getAttackBox().actual.size.width,_cherry->getAttackBox().actual.origin.y + _cherry->getAttackBox().actual.size.height);
		//circle
//		CCPoint p1 = ccp(_cherry->getCircleAttackBox().actual.origin.x,_cherry->getCircleAttackBox().actual.origin.y);
//		CCPoint p2 = ccp(_cherry->getCircleAttackBox().actual.origin.x + _cherry->getCircleAttackBox().actual.size.width,_cherry->getCircleAttackBox().actual.origin.y + _cherry->getCircleAttackBox().actual.size.height);
//
//		CCPoint p1 = ccp(_cherry->getSplitAttackBox().actual.origin.x,_cherry->getSplitAttackBox().actual.origin.y);
//		CCPoint p2 = ccp(_cherry->getSplitAttackBox().actual.origin.x + _cherry->getSplitAttackBox().actual.size.width,_cherry->getSplitAttackBox().actual.origin.y + _cherry->getSplitAttackBox().actual.size.height);
//
//		ccDrawColor4B(0, 0, 0, 255);
//		ccDrawRect(p1, p2);

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

	if (sprite->getTag() == 1)  // target
	{
//		cocos2d::CCLog("enemy->hurtWithDamage(_cherry->getDamage());");

	} else if (sprite->getTag() == 2) // projectile
	{
		_projectiles->removeObject(sprite);

		if (m_emitter->isActive())
			this->removeChild(m_emitter, true);

		m_emitter = new CCParticleSystemQuad();
		std::string filename = "Particles/Phoenix.plist";
		m_emitter->initWithFile(filename.c_str());

		// texture
		m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));

		// additive
		m_emitter->setBlendAdditive(true);
		m_emitter->setLife(0.1f);
		m_emitter->setLifeVar(0.1f);
		m_emitter->setDuration(0.1f);

		m_emitter->setPosition(sprite->getPositionX(), sprite->getPositionY());
		m_emitter->setAutoRemoveOnFinish(true);

		this->addChild(m_emitter, 20);

		cocos2d::CCLog("_projectiles->removeObject(sprite);");
	} else if (sprite->getTag() == 3)
	{
		this->removeChild(sprite);
	}
}

void GameLayer::endGame()
{
	CCMenuItemImage *goBack = CCMenuItemImage::create("retryOn.png", "retryOff.png", this, menu_selector(GameLayer::restartGame));

// Place the menu item bottom-right conner.
	goBack->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));

	CCMenu* pMenu = CCMenu::create(goBack, NULL);
	pMenu->setPosition(CCPointZero);

	pMenu->setTag(50);
	_hud->addChild(pMenu, 50);
}

void GameLayer::restartGame(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameScene::create());
}

void GameLayer::mainMenu()
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
