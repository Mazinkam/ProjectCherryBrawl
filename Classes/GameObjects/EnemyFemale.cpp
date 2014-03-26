/*
 * EnemyFemale.cpp
 *
 *  Created on: 9.3.2014
 *      Author: user
 */

#include "EnemyFemale.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

EnemyFemale::EnemyFemale(void)
{
}

EnemyFemale::~EnemyFemale(void)
{
}

bool EnemyFemale::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("fenemy_idle001.png"));

		int i;
		//idle animation
		CCArray *idleFrames = CCArray::createWithCapacity(6);
		for (i = 1; i < 7; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("fenemy_idle%03d.png", i)->getCString());
			idleFrames->addObject(frame);
		}
		CCAnimation *idleAnimation = CCAnimation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
		this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

		//attack animation
		CCArray *attackFrames = CCArray::createWithCapacity(17);
		for (i = 1; i < 18; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fenemy_attack%03d.png", i)->getCString());
			frame->setOffset(ccp(18,0));
			attackFrames->addObject(frame);
		}
		CCAnimation *attackAnimation = CCAnimation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
		this->setAttackAction(CCSequence::create(CCAnimate::create(attackAnimation), CCCallFunc::create(this, callfunc_selector(EnemyFemale::idle)), NULL));

		//walk animation
		CCArray *walkFrames = CCArray::createWithCapacity(12);
		for (i = 1; i < 13; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("fenemy_walk%03d.png", i)->getCString());
			walkFrames->addObject(frame);
		}
		CCAnimation *walkAnimation = CCAnimation::createWithSpriteFrames(walkFrames, float(1.0 / 12.0));
		this->setWalkAction(CCRepeatForever::create(CCAnimate::create(walkAnimation)));

		//hurt animation
		CCArray *hurtFrames = CCArray::createWithCapacity(2);
		for (i = 1; i < 3; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fenemy_hit%03d.png", i)->getCString());
			hurtFrames->addObject(frame);

		}
		CCAnimation *hurtAnimation = CCAnimation::createWithSpriteFrames(hurtFrames, float(1.0 / 12.0));
		this->setHurtAction(CCSequence::create(CCAnimate::create(hurtAnimation), CCCallFunc::create(this, callfunc_selector(EnemyFemale::idle)), NULL));

		//splitting animation
		CCArray *splittingFrames = CCArray::createWithCapacity(5);
		for (i = 1; i < 6; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("fenemy_split%03d.png", i)->getCString());
			splittingFrames->addObject(frame);

		}
		CCAnimation *splittingAnimation = CCAnimation::createWithSpriteFrames(splittingFrames, float(1.0 / 12.0));
		this->setSplittingAction(CCSequence::create(CCAnimate::create(splittingAnimation), CCFadeOut::create(3), CCRemoveSelf::create(true), NULL));

		//knocked out animation
		CCArray *knockedOutFrames = CCArray::createWithCapacity(4);
		for (i = 1; i < 5; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fenemy_hit%03d.png", i)->getCString());
			if (i > 3)
				frame->setOffset(ccp(0,-10));
			knockedOutFrames->addObject(frame);
		}
		CCAnimation *knockedOutAnimation = CCAnimation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
		this->setKnockedOutAction(CCSequence::create(CCAnimate::create(knockedOutAnimation), CCBlink::create(2.0, 10.0), CCRemoveSelf::create(true), NULL));

		//WalkIdle
	//	this->setWalkIdle(CCSequence::create(CCAnimate::create(walkAnimation), CCCallFunc::create(this, callfunc_selector(EnemyFemale::idle)), NULL));


		this->setWalkSpeed(80.0);
		this->setCenterToBottom(39.0);
		this->setCenterToSides(29.0);
		this->setHitPoints(100.0);
		this->setDamage(5.0);

		this->setSpriteType("Common");

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

void EnemyFemale::knockout()
{
	ActionSprite::knockout();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("death.wav");
}

