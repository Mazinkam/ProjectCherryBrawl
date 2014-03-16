/*
 * Cherry.cpp
 *
 *  Created on: 3.3.2014
 *      Author: user
 */

#include "Cherry.h"
#include "SimpleAudioEngine.h"
#include "../GameResources.h"
using namespace cocos2d;

Cherry::Cherry(void)
{

}

Cherry::~Cherry(void)
{
}

bool Cherry::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("idle001.png"));

		int i;
		//idle animation
		CCArray *idleFrames = CCArray::createWithCapacity(12);
		for (i = 1; i < 13; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("idle%03d.png", i)->getCString());
			idleFrames->addObject(frame);

//			cocos2d::CCLog("idle%03d.png", i);
		}
		CCAnimation *idleAnimation = CCAnimation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
		this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

//		attack animation
		CCArray *attackFrames = CCArray::createWithCapacity(10);
		for (i = 1; i < 11; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("skill1_%03d.png", i)->getCString());
			frame->setOffset(ccp(45,24));
			attackFrames->addObject(frame);

		}
		CCAnimation *attackAnimation = CCAnimation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
		this->setAttackAction(CCSequence::create(CCAnimate::create(attackAnimation), CCCallFunc::create(this, callfunc_selector(Cherry::idle)), NULL));

//walk animation
		CCArray *walkFrames = CCArray::createWithCapacity(12);
		for (i = 1; i < 13; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("walk%03d.png", i)->getCString());
			walkFrames->addObject(frame);
			//			cocos2d::CCLog("walk%03d.png ", i);
		}
		CCAnimation *walkAnimation = CCAnimation::createWithSpriteFrames(walkFrames, float(1.0 / 12.0));
		this->setWalkAction(CCRepeatForever::create(CCAnimate::create(walkAnimation)));
//hurt animation
		CCArray *hurtFrames = CCArray::createWithCapacity(1);
		for (i = 1; i < 2; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("hit%03d.png", i)->getCString());
			hurtFrames->addObject(frame);
		}
		CCAnimation *hurtAnimation = CCAnimation::createWithSpriteFrames(hurtFrames, float(1.0 / 12.0));
		this->setHurtAction(CCSequence::create(CCAnimate::create(hurtAnimation), CCCallFunc::create(this, callfunc_selector(Cherry::idle)), NULL));

//knocked out animation
		CCArray *knockedOutFrames = CCArray::createWithCapacity(3);
		for (i = 1; i < 4; i++)
		{

			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("hit%03d.png", i)->getCString());
			if (i == 3)
				frame->setOffset(ccp(0,-10));
			knockedOutFrames->addObject(frame);
		}
		CCAnimation *knockedOutAnimation = CCAnimation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
		this->setKnockedOutAction(CCSequence::create(CCAnimate::create(knockedOutAnimation), CCBlink::create(2.0, 10.0), NULL));

		this->setCenterToBottom(39.0);
		this->setCenterToSides(29.0);
		this->setHitPoints(90.0);
		this->setManaPool(3);
		this->setLife(3);
		this->setDamage(20.0);
		this->setProjectileDamage(80.0);
		this->setCircleDamage(40.0);
		this->setWalkSpeed(120.0);

		this->setHitbox(this->createBoundingBoxWithOrigin(ccp(-this->getCenterToSides(),
				-this->getCenterToBottom()), CCSizeMake(this->getCenterToSides() * 2, this->getCenterToBottom() * 2)));

		this->setAttackBox(this->createBoundingBoxWithOrigin(ccp(this->getCenterToSides(), -this->getCenterToBottom()), CCSizeMake(40, this->getCenterToBottom()*2)));

		this->setCircleAttackBox(this->createBoundingBoxWithOrigin(ccp(-this->getCenterToSides()*6, -this->getCenterToBottom()*6), CCSizeMake(this->getCenterToSides()*13, this->getCenterToBottom()*10)));

		bRet = true;
	} while (0);

	return bRet;
}

void Cherry::knockout()
{
	ActionSprite::knockout();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("death.wav");
}

