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
//			cocos2d::CCLog("1 fenemy_idle%03d.png", i);
		}
		CCAnimation *idleAnimation = CCAnimation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
		this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

		//attack animation
		CCArray *attackFrames = CCArray::createWithCapacity(17);
		for (i = 1; i < 18; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fenemy_attack%03d.png", i)->getCString());
			frame->setOffset(ccp(13,0));
			attackFrames->addObject(frame);
//			cocos2d::CCLog("Fenemy_attack%02d.png", i);

		}
		CCAnimation *attackAnimation = CCAnimation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
		this->setAttackAction(CCSequence::create(CCAnimate::create(attackAnimation), CCCallFunc::create(this, callfunc_selector(EnemyFemale::idle)), NULL));

		//walk animation
		CCArray *walkFrames = CCArray::createWithCapacity(12);
		for (i = 1; i < 13; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("fenemy_walk%03d.png", i)->getCString());
			walkFrames->addObject(frame);
//			cocos2d::CCLog("fenemy_walk%02d.png", i);
		}
		CCAnimation *walkAnimation = CCAnimation::createWithSpriteFrames(walkFrames, float(1.0 / 12.0));
		this->setWalkAction(CCRepeatForever::create(CCAnimate::create(walkAnimation)));

		//hurt animation
		CCArray *hurtFrames = CCArray::createWithCapacity(1);
		for (i = 1; i < 2; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fenemy_hit%03d.png", i)->getCString());
			hurtFrames->addObject(frame);
//			cocos2d::CCLog("Fenemy_hit%02d.png", i);

		}
		CCAnimation *hurtAnimation = CCAnimation::createWithSpriteFrames(hurtFrames, float(1.0 / 12.0));
		this->setHurtAction(CCSequence::create(CCAnimate::create(hurtAnimation), CCCallFunc::create(this, callfunc_selector(EnemyFemale::idle)), NULL));

		//knocked out animation
		CCArray *knockedOutFrames = CCArray::createWithCapacity(4);
		for (i = 1; i < 5; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Fenemy_hit%03d.png", i)->getCString());
			if(i > 3)
				frame->setOffset(ccp(0,-10));
			knockedOutFrames->addObject(frame);
//			cocos2d::CCLog("Fenemy_hit%02d.png", i);

		}
		CCAnimation *knockedOutAnimation = CCAnimation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
		this->setKnockedOutAction(CCSequence::create(CCAnimate::create(knockedOutAnimation), CCBlink::create(2.0, 10.0), CCRemoveSelf::create(), NULL));

		this->setWalkSpeed(80.0);
		this->setCenterToBottom(39.0);
		this->setCenterToSides(29.0);
		this->setHitPoints(100.0);
		this->setDamage(10.0);

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

