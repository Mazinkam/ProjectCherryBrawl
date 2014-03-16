#include "../Layers/GameLayer.h"
#include "../GameObjects/EnemyFemale.h"
#include "../Scenes/GameScene.h"
#include "SimpleAudioEngine.h"
#include "../Defines.h"

using namespace cocos2d;
bool showHitBox = true;

int _skillButtonSize = 76;
int _betweenButtons = 60;
int _sizeFromSide = 52;
int _sidePadding = 20;
int _horizPadding = 10;

int _blipSize = 32;
int _blipSpaceSize = 7;

int _manaSize = 48;
int _manaPadding = 15;
int _manaSpaceSize = 13;

int _lifeSize = 50;
int _lifeSpaceSize = 10;

GameLayer::GameLayer(void) :
		_projectiles(NULL)
{
	_tileMap = NULL;
	_cherry = NULL;
	_enemies = NULL;
	_manaPool = NULL;
	_bInit = false;

}

GameLayer::~GameLayer(void)
{
	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}

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
		_manaEmptyBall = CCTextureCache::sharedTextureCache()->addImage(s_ManaEmpty);
		_manaFullBall = CCTextureCache::sharedTextureCache()->addImage(s_ManaFull);

		_projectiles = new CCArray;
		_manaPool = new CCArray;
		_hpPool = new CCArray;

		this->setTouchEnabled(true);

		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile(s_gameAtlasPlist, s_gameAtlasPNG);

		_actorsTest = CCSpriteBatchNode::create(s_gameAtlasPNG);

		_actorsTest->getTexture()->setAntiAliasTexParameters();
		this->addChild(_actorsTest, -5);

		this->initTileMap();
		this->initEnemies();
		this->initCherry();

		this->scheduleUpdate();
		this->setKeypadEnabled(true);

		m_emitter = new CCParticleSystemQuad();

//		this->initSkillBar();
		bRet = true;
	} while (0);
//_hud->get
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

void GameLayer::initCherry()
{
	_cherry = Cherry::create();
	_actorsTest->addChild(_cherry);
	_cherry->setPosition(ccp(_cherry->getCenterToSides(), 80));
	_cherry->setDesiredPosition(_cherry->getPosition());
	_cherry->idle();
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}

//
void GameLayer::didChangeDirectionTo(SimpleDPad *simpleDPad, CCPoint direction)
{
	_cherry->walkWithDirection(direction);
}

void GameLayer::isHoldingDirection(SimpleDPad *simpleDPad, CCPoint direction)
{
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
	updateUI();

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
					projectile->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(GameLayer::projectileMoveFinished)), NULL));
					cocos2d::CCLog("enemy->hurtWithDamage(_cherry->getDamage());");
				}
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
			MAX(_cherry->getCenterToSides(), _cherry->getDesiredPosition().x));
	float posY = MIN(3 * _tileMap->getTileSize().height + _cherry->getCenterToBottom(),
			MAX(_cherry->getCenterToBottom()+5, _cherry->getDesiredPosition().y));
	_cherry->setPosition(ccp(posX, posY));

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
	CCARRAY_FOREACH(_manaPool, item)
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
	}

	i = 0;
	CCARRAY_FOREACH(_hpPool, item)
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

void GameLayer::initEnemies()
{
	int enemyCount = 25;
	this->setEnemies(CCArray::createWithCapacity(enemyCount));

	for (int i = 0; i < enemyCount; i++)
	{
		EnemyFemale *enemy = EnemyFemale::create();
		_actorsTest->addChild(enemy);
		_enemies->addObject(enemy);

		int minX = SCREEN.width + enemy->getCenterToSides();
		int maxX = _tileMap->getMapSize().width * _tileMap->getTileSize().width - enemy->getCenterToSides();
		int minY = enemy->getCenterToBottom();
		int maxY = 3 * _tileMap->getTileSize().height + enemy->getCenterToBottom();
		enemy->setScaleX(-1);
		enemy->setPosition(ccp(random_range(minX, maxX), random_range(minY, maxY)));
		enemy->setDesiredPosition(enemy->getPosition());
		enemy->idle();
	}

}

void GameLayer::reorderActors()
{
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_actorsTest->getChildren(), pObject)
	{
		ActionSprite *sprite = (ActionSprite*) pObject;
		_actorsTest->reorderChild(sprite, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - sprite->getPosition().y);
	}
}

void GameLayer::updateEnemies(float dt)
{
	int alive = 0;
	float distanceSQ;
	int randomChoice = 0;
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_enemies, pObject)
	{
		EnemyFemale *enemy = (EnemyFemale*) pObject;
		enemy->update(dt);
		if (enemy->getActionState() != kActionStateKnockedOut)
		{
			//1
			alive++;

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
								if (_cherry->getHitbox().actual.intersectsRect(enemy->getAttackBox().actual))
								{
									_cherry->hurtWithDamage(enemy->getDamage());

									//end game
									if (_cherry->getActionState() == kActionStateKnockedOut && _hud->getChildByTag(5) == NULL)
									{
										this->endGame();
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
					enemy->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5) * 1000);
					randomChoice = random_range(0, 2);
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

				//end game checker here
	if (alive == 0 && _hud->getChildByTag(5) == NULL)
	{
		this->endGame();
	}

	this->initSkillBar();
//	_hud->PassingObjects(_cherry, _enemies);
}

void GameLayer::initSkillBar()
{
	if (_hud != NULL && _hud->getChildByTag(10) == NULL && _bInit == false)
	{
		CCMenuItemImage *_goBack = CCMenuItemImage::create(s_PauseOff, s_PauseOn, this, menu_selector(GameLayer::mainMenu));

		_goBack->setPosition(ccp(SCREEN.width - 20, SCREEN.height-20));

		CCMenuItemImage *_skillOne = CCMenuItemImage::create(s_Skill_1_Off, s_Skill_1_On, this, menu_selector(GameLayer::firstSkill));
		_skillOne->setPosition(ccp(SCREEN.width - (_betweenButtons ),_sizeFromSide));

		CCMenuItemImage *_skillTwo = CCMenuItemImage::create(s_Skill_2_Off, s_Skill_2_On, this, menu_selector(GameLayer::projectileSkill));
		_skillTwo->setPosition(ccp(SCREEN.width - (_betweenButtons + _sidePadding + _skillButtonSize ), _sizeFromSide));

		CCMenuItemImage *_skillThree = CCMenuItemImage::create(s_Skill_3_Off, s_Skill_3_On, this, menu_selector(GameLayer::circleSkill));
		_skillThree->setPosition(ccp(SCREEN.width - (_betweenButtons ), _sizeFromSide + _horizPadding + _skillButtonSize));

		CCMenuItemImage *_skillFour = CCMenuItemImage::create(s_Skill_4_Off, s_Skill_4_On, this, menu_selector(GameLayer::firstSkill));
		_skillFour->setPosition(ccp(SCREEN.width - (_betweenButtons + _sidePadding + _skillButtonSize ), _sizeFromSide + _horizPadding + _skillButtonSize));

		CCMenu* pMenu = CCMenu::create(_skillOne, _skillTwo, _skillThree, _skillFour, _goBack, NULL);
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
			_manaOrb->setPosition(ccp(SCREEN.width/3.35+ ((_manaSize * i) + (_manaSpaceSize*i)), SCREEN.height/11));
			_manaOrb->setTag(i);
			_manaPool->addObject(_manaOrb);
			_hud->addChild(_manaOrb, 11);
		}

		for (int i = 0; i <= _cherry->getHitPoints() / 10; i++)
		{
			CCSprite *_hpBlip = CCSprite::create();
			if (i * 10 <= _cherry->getHitPoints())
			{
				_hpBlip->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_HpBlip));
			}
			_hpBlip->setPosition(ccp(SCREEN.width/3.29+ ((_blipSize * i) + (_blipSpaceSize*i)) - _horizPadding, SCREEN.height/4.8));
			_hpBlip->setTag(i);
			_hpPool->addObject(_hpBlip);
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

	}

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
	}

}

//void GameLayer::projectileSkill()
//{
//	if (_cherry->getManaPool() >= 2)
//	{
//		_cherry->setManaPool(_cherry->getManaPool() - 2);
//		_cherry->projectileAttack();
//
//		updateUI();
//
//		m_emitter = new CCParticleSystemQuad();
//		std::string filename = "Particles/Phoenix.plist";
//		m_emitter->initWithFile(filename.c_str());
//
//		// texture
//		m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_Stars));
//
//		// additive
//		m_emitter->setBlendAdditive(true);
//		m_emitter->setDuration(1.2f);
//
//		m_emitter->setPosition(_cherry->getScaleX() == -1 ? (projectile->getPositionX() - 20) : (projectile->getPositionX() + 50), projectile->getPositionY());
//		m_emitter->setAutoRemoveOnFinish(true);
//
//		this->addChild(m_emitter, 20);
//		m_emitter->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), NULL));
//	}
//
//}

void GameLayer::projectileSkill()
{
	if (_cherry->getManaPool() >= 2)
	{
		_cherry->setManaPool(_cherry->getManaPool() - 2);
		_cherry->projectileAttack();

		updateUI();
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

		float velocity = 480 / 1; // 480pixels/1sec

		float realMoveDuration = length / velocity;

		// Move projectile to actual endpoint
		projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest), CCBlink::create(1.0, 3.0), CCCallFuncN::create(this, callfuncN_selector(GameLayer::projectileMoveFinished)), NULL));

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
		updateUI();
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
////		CCPoint p1 = ccp(_cherry->getProjectileAttackBox().actual.origin.x,_cherry->getProjectileAttackBox().actual.origin.y);
////		CCPoint p2 = ccp(_cherry->getProjectileAttackBox().actual.origin.x + _cherry->getProjectileAttackBox().actual.size.width,_cherry->getProjectileAttackBox().actual.origin.y + _cherry->getProjectileAttackBox().actual.size.height);
////
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
	}
}

void GameLayer::projectileMoveFinished(CCNode* sender)
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
	}
}

void GameLayer::endGame()
{
	CCMenuItemImage *goBack = CCMenuItemImage::create("retryOn.png", "retryOff.png", this, menu_selector(GameLayer::restartGame));

// Place the menu item bottom-right conner.
	goBack->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));

	CCMenu* pMenu = CCMenu::create(goBack, NULL);
	pMenu->setPosition(CCPointZero);

	pMenu->setTag(5);
	_hud->addChild(pMenu, 5);
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
