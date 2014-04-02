/*
 * ObjectShadow.cpp
 *
 *  Created on: 2.4.2014
 *      Author: user
 */

#include "ObjectShadow.h"
#include "cocos2d.h"

using namespace cocos2d;

ObjectShadow::ObjectShadow()
{
	// TODO Auto-generated constructor stub

}

ObjectShadow::~ObjectShadow()
{
	// TODO Auto-generated destructor stub
}
bool ObjectShadow::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName(s_GameShadow));

		int i;
		//idle animation
		CCArray *idleFrames = CCArray::createWithCapacity(1);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_GameShadow);
		idleFrames->addObject(frame);
		CCAnimation *idleAnimation = CCAnimation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
		this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

		//knocked out animation
		CCArray *knockedOutFrames = CCArray::createWithCapacity(1);
		knockedOutFrames->addObject(frame);
		CCAnimation *knockedOutAnimation = CCAnimation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
		this->setKnockedOutAction(CCSequence::create(CCAnimate::create(knockedOutAnimation), CCFadeOut::create(0.5f), NULL));

		//WalkIdle
		//	this->setWalkIdle(CCSequence::create(CCAnimate::create(walkAnimation), CCCallFunc::create(this, callfunc_selector(ObjectShadow::idle)), NULL));

		this->setWalkSpeed(200.0);
		this->setCenterToSides(29.0);
		this->setSpriteType("Wings");

		bRet = true;
	} while (0);

	return bRet;
}

void ObjectShadow::knockout()
{
	ActionSprite::knockout();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("death.wav");
}
