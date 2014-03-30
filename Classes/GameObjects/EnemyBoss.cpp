/*
 * EnemyBoss.cpp
 *
 *  Created on: 19.3.2014
 *      Author: user
 */

#include "EnemyBoss.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

EnemyBoss::EnemyBoss()
{
	// TODO Auto-generated constructor stub

}

EnemyBoss::~EnemyBoss()
{
	// TODO Auto-generated destructor stub
}
bool EnemyBoss::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("boss_idle001.png"));

		int i;
		//idle animation
		CCArray *idleFrames = CCArray::createWithCapacity(6);
		for (i = 1; i < 7; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("boss_idle%03d.png", i)->getCString());
			idleFrames->addObject(frame);
		}
		CCAnimation *idleAnimation = CCAnimation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
		this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

		//attack animation
		CCArray *attackFrames = CCArray::createWithCapacity(16);
		for (i = 1; i < 17; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("boss_attack%03d.png", i)->getCString());
			frame->setOffset(ccp(18,0));
			attackFrames->addObject(frame);
		}
		CCAnimation *attackAnimation = CCAnimation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
		this->setAttackAction(CCSequence::create(CCAnimate::create(attackAnimation), CCCallFunc::create(this, callfunc_selector(EnemyBoss::idle)), NULL));

		//attack Projectile animation
		CCArray *attackProjectileFrames = CCArray::createWithCapacity(16);
		for (i = 1; i < 17; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("boss_attack%03d.png", i)->getCString());
			//frame->setOffset(ccp(45,24));
			attackProjectileFrames->addObject(frame);

		}
		CCAnimation *attackProjectileAnimation = CCAnimation::createWithSpriteFrames(attackProjectileFrames, float(1.0 / 6.0));
		this->setProjectileAttackAction(CCSequence::create(CCAnimate::create(attackProjectileAnimation), CCCallFunc::create(this, callfunc_selector(EnemyBoss::idle)), NULL));


		//walk animation
		CCArray *walkFrames = CCArray::createWithCapacity(2);
		for (i = 1; i < 3; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("boss_attack%03d.png", i)->getCString());
			walkFrames->addObject(frame);
		}
		CCAnimation *walkAnimation = CCAnimation::createWithSpriteFrames(walkFrames, float(1.0 / 12.0));
		this->setWalkAction(CCRepeatForever::create(CCAnimate::create(walkAnimation)));

		//hurt animation
		CCArray *hurtFrames = CCArray::createWithCapacity(1);
		for (i = 1; i < 2; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("boss_hit%03d.png", i)->getCString());
			hurtFrames->addObject(frame);

		}
		CCAnimation *hurtAnimation = CCAnimation::createWithSpriteFrames(hurtFrames, float(1.0 / 12.0));
		this->setHurtAction(CCSequence::create(CCAnimate::create(hurtAnimation), CCCallFunc::create(this, callfunc_selector(EnemyBoss::idle)), NULL));

		//knocked out animation
		CCArray *knockedOutFrames = CCArray::createWithCapacity(8);
		for (i = 1; i < 9; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("bossdead_%03d.png", i)->getCString());
			knockedOutFrames->addObject(frame);
			LOG("sprite num: %i",  i);
		}
		CCAnimation *knockedOutAnimation = CCAnimation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
		this->setKnockedOutAction(CCSequence::create(CCAnimate::create(knockedOutAnimation), NULL));

		//WalkIdle
		//	this->setWalkIdle(CCSequence::create(CCAnimate::create(walkAnimation), CCCallFunc::create(this, callfunc_selector(EnemyBoss::idle)), NULL));

		this->setWalkSpeed(200.0);
		this->setCenterToBottom(39.0);
		this->setCenterToSides(29.0);
		this->setHitPoints(500.0);
		this->setDamage(10.0);

		this->setSpriteType("Boss");

		this->setHitbox(this->createBoundingBoxWithOrigin(ccp(-this->getCenterToSides(),
				-this->getCenterToBottom()),
		CCSizeMake(this->getCenterToSides() * 2,
				this->getCenterToBottom() * 2)));
		this->setAttackBox(this->createBoundingBoxWithOrigin(ccp(this->getCenterToSides(), -10), CCSizeMake(30, 20)));

		_nextDecisionTime = 0;

		bRet = true;
	} while (0);

	return bRet;
}

void EnemyBoss::knockout()
{
	ActionSprite::knockout();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("death.wav");
}

