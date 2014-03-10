#include "../Layers/GameLayer.h"
#include "../GameObjects/EnemyFemale.h"
#include "../Scenes/GameScene.h"
#include "SimpleAudioEngine.h"
#include "../Defines.h"

using namespace cocos2d;

GameLayer::GameLayer(void)
{
	_tileMap = NULL;
	_cherry = NULL;
	_enemies = NULL;
	_bInit = false;

}

GameLayer::~GameLayer(void)
{
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
		this->setTouchEnabled(true);

		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("game_atlas.plist", "game_atlas.png");

		_actorsTest = CCSpriteBatchNode::create("game_atlas.png");

		_actorsTest->getTexture()->setAntiAliasTexParameters();
		this->addChild(_actorsTest, -5);

		this->initTileMap();
		this->initEnemies();
		this->initCherry();

		this->scheduleUpdate();
		this->setKeypadEnabled(true);

//		this->initSkillBar();
		bRet = true;
	} while (0);
//_hud->get
	return bRet;
}

void GameLayer::initTileMap()
{
	_tileMap = CCTMXTiledMap::create("testmap.tmx");
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

	_cherry->attack();

	if (_cherry->getActionState() == kActionStateAttack)
	{
		CCObject *pObject = NULL;
		CCARRAY_FOREACH(_enemies, pObject)
		{
			EnemyFemale *enemy = (EnemyFemale*) pObject;
			if (enemy->getActionState() != kActionStateKnockedOut)
			{
				if (fabsf(_cherry->getPosition().y - enemy->getPosition().y) < 10)
				{
					if (_cherry->getAttackBox().actual.intersectsRect(enemy->getHitbox().actual))
					{
						enemy->hurtWithDamage(_cherry->getDamage());
					}
				}
			}
		}
	}
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
	int enemyCount = 5;
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
		int _skillButtonSize = 76;
		int _betweenButtons = 60;
		int _sizeFromSide = 52;
		int _sidePadding = 20;
		int _horizPadding = 10;

		int _blipSize = 32;
		int _blipSpaceSize = 7;

		int _manaSize = 48;
		int _manaPadding = 15;
		int _manaSpaceSize = 15;

		int _lifeSize = 50;
		int _lifeSpaceSize = 10;

		CCMenuItemImage *_goBack = CCMenuItemImage::create("PAUSE_off.png", "PAUSE_on.png", this, menu_selector(GameLayer::mainMenu));

		_goBack->setPosition(ccp(SCREEN.width - 20, SCREEN.height-20));

		CCMenuItemImage *_skillOne = CCMenuItemImage::create("SKILL1_off.png", "SKILL1_on.png", this, menu_selector(GameLayer::firstSkill));
		_skillOne->setPosition(ccp(SCREEN.width - (_betweenButtons ),_sizeFromSide));

		CCMenuItemImage *_skillTwo = CCMenuItemImage::create("SKILL2_off.png", "SKILL2_on.png", this, menu_selector(GameLayer::firstSkill));
		_skillTwo->setPosition(ccp(SCREEN.width - (_betweenButtons + _sidePadding + _skillButtonSize ), _sizeFromSide));

		CCMenuItemImage *_skillThree = CCMenuItemImage::create("SKILL3_off.png", "SKILL3_on.png", this, menu_selector(GameLayer::circleSkill));
		_skillThree->setPosition(ccp(SCREEN.width - (_betweenButtons ), _sizeFromSide + _horizPadding + _skillButtonSize));

		CCMenuItemImage *_skillFour = CCMenuItemImage::create("SKILL4_off.png", "SKILL4_on.png", this, menu_selector(GameLayer::firstSkill));
		_skillFour->setPosition(ccp(SCREEN.width - (_betweenButtons + _sidePadding + _skillButtonSize ), _sizeFromSide + _horizPadding + _skillButtonSize));

		CCMenu* pMenu = CCMenu::create(_skillOne, _skillTwo, _skillThree, _skillFour, _goBack, NULL);
		pMenu->setPosition(CCPointZero);

		pMenu->setTag(10);
		_hud->addChild(pMenu, 10);


		CCSprite *_manaFull = CCSprite::create("MANA_full.png");
		_manaFull->setPosition(ccp(SCREEN.width/3.35, SCREEN.height/11));

		CCSprite *_manaEmpty = CCSprite::create("MANA_empty.png");
		_manaEmpty->setPosition(ccp(SCREEN.width/3.35+ ((_manaSize * 1) + (_manaSpaceSize*1)), SCREEN.height/11));

		CCSprite *_hpBlip = CCSprite::create("HP_blip.png");
		_hpBlip->setPosition(ccp(SCREEN.width/2 - ((_blipSize * 4) + (_blipSpaceSize*4)) - _horizPadding, SCREEN.height/4.8));


		CCSprite *_playerLife = CCSprite::create("LIFE.png");
		_playerLife->setPosition(ccp(SCREEN.width/3.35 + ((_lifeSize * 0) + (_lifeSpaceSize*0)), SCREEN.height/3.2));


		_hud->addChild(_manaFull,11);
		_hud->addChild(_manaEmpty,11);
		_hud->addChild(_playerLife,11);
		_hud->addChild(_hpBlip,7);

		_bInit = true;

	}

}

void GameLayer::firstSkill()
{
	_cherry->attack();

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
					}
				}
			}
		}
	}
}

void GameLayer::circleSkill()
{
	_cherry->knockout();

	if (_cherry->getActionState() == kActionStateAttack)
	{
		CCObject *pObject = NULL;
		CCARRAY_FOREACH(_enemies, pObject)
		{
			EnemyFemale *enemy = (EnemyFemale*) pObject;
			if (enemy->getActionState() != kActionStateKnockedOut)
			{
				if (fabsf(_cherry->getPosition().y - enemy->getPosition().y) < 40)
				{
					if (_cherry->getCircleAttackBox().actual.intersectsRect(enemy->getHitbox().actual))
					{
						enemy->hurtWithDamage(_cherry->getDamage());
					}
				}
			}
		}
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
void GameLayer::keyBackClicked(void) {
    CCDirector::sharedDirector()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}
