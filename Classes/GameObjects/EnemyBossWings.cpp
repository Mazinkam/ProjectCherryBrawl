/*
 * EnemyBossWingsWings.cpp
 *
 *  Created on: 30.3.2014
 *      Author: user
 */

#include "EnemyBossWings.h"
//#include "SimpleAudioEngine.h"
using namespace cocos2d;

EnemyBossWings::EnemyBossWings()
{
	// TODO Auto-generated constructor stub

}

EnemyBossWings::~EnemyBossWings()
{
	// TODO Auto-generated destructor stub
}
bool EnemyBossWings::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("wing_001.png"));

		int i;
		//idle animation
		CCArray *idleFrames = CCArray::createWithCapacity(6);
		for (i = 1; i < 5; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("wing_%03d.png", i)->getCString());
			idleFrames->addObject(frame);
		}
		CCAnimation *idleAnimation = CCAnimation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
		this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

		//knocked out animation
		CCArray *knockedOutFrames = CCArray::createWithCapacity(6);
		for (i = 1; i < 7; i++)
		{
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("wingdead_%03d.png", i)->getCString());
			knockedOutFrames->addObject(frame);
		}
		CCAnimation *knockedOutAnimation = CCAnimation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
		this->setKnockedOutAction(CCSequence::create(CCAnimate::create(knockedOutAnimation), CCFadeOut::create(0.5f), NULL));

		//WalkIdle
		//	this->setWalkIdle(CCSequence::create(CCAnimate::create(walkAnimation), CCCallFunc::create(this, callfunc_selector(EnemyBossWings::idle)), NULL));

		this->setWalkSpeed(200.0);
		this->setCenterToSides(29.0);
		this->setSpriteType("Wings");

		bRet = true;
	} while (0);

	return bRet;
}

void EnemyBossWings::knockout()
{
	ActionSprite::knockout();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("death.wav");
}
