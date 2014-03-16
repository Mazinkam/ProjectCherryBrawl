/*
 * ActionSprite.cpp
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#include "ActionSprite.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

ActionSprite::ActionSprite(void)
{

	//behaviour
	_idleAction = NULL;
	_walkAction = NULL;
	_hurtAction = NULL;
	_knockedOutAction = NULL;

	//attacks
	_attackAction = NULL;
	_circleAttackAction = NULL;
	_projectileAttackAction = NULL;

}

ActionSprite::~ActionSprite(void)
{
}

void ActionSprite::idle()
{
	if (_actionState != kActionStateIdle)
	{
		this->stopAllActions();
		this->runAction(_idleAction);
		_actionState = kActionStateIdle;
		_velocity = CCPointZero; // same as 0,0
	}
}
//add more states for different atks and sword movements
void ActionSprite::attack()
{
	if (_actionState == kActionStateIdle || _actionState != kActionStateAttack || _actionState == kActionStateWalk)
	{
		this->stopAllActions();
		this->runAction(_attackAction);
		_actionState = kActionStateAttack;
	}
}

void ActionSprite::circleAttack()
{
	if (_actionState == kActionStateIdle || _actionState != kActionStateAttack || _actionState == kActionStateWalk)
	{
		this->stopAllActions();
		this->runAction(_attackAction);
		_actionState = kActionStateAttack;
	}
}

void ActionSprite::projectileAttack()
{
	if (_actionState == kActionStateIdle || _actionState != kActionStateAttack  || _actionState == kActionStateWalk)
	{
		this->stopAllActions();
		this->runAction(_attackAction);
		_actionState = kActionStateAttack;
	}
}

void ActionSprite::hurtWithDamage(float damage)
{
	if (_actionState != kActionStateKnockedOut)
	{
		int randomSound = random_range(0, 1);
		//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("hit%d.wav", randomSound)->getCString());
		this->stopAllActions();
		this->runAction(_hurtAction);
		_actionState = kActionStateHurt;
		_hitPoints -= damage;

		if (_hitPoints <= 0)
		{
			this->knockout();
		}
	}
}

void ActionSprite::knockout()
{
	this->stopAllActions();
	this->runAction(_knockedOutAction);
	_hitPoints = 0;
	_actionState = kActionStateKnockedOut; // k prefix is konstant!, _ for memeber variables
}

void ActionSprite::walkWithDirection(CCPoint direction)
{
	if (_actionState == kActionStateIdle)
	{
		this->stopAllActions();
		this->runAction(_walkAction);
		_actionState = kActionStateWalk;
	}
	if (_actionState == kActionStateWalk)
	{
		_velocity = ccp(direction.x * _walkSpeed, direction.y * _walkSpeed);
		if (_velocity.x >= 0)
		{
			this->setScaleX(1.0);
		}
		else
		{
			this->setScaleX(-1.0);
		}
	}
}

void ActionSprite::update(float dt)
{
	if (_actionState == kActionStateWalk)
	{
		_desiredPosition = ccpAdd(this->getPosition(), ccpMult(_velocity, dt));
	}
}

BoundingBox ActionSprite::createBoundingBoxWithOrigin(CCPoint origin, CCSize size)
{
	BoundingBox boundingBox;
	boundingBox.original.origin = origin;
	boundingBox.original.size = size;
	boundingBox.actual.origin = ccpAdd(this->getPosition(), ccp(boundingBox.original.origin.x, boundingBox.original.origin.y));
	boundingBox.actual.size = size;
	return boundingBox;
}
//here for different attacks
void ActionSprite::transformBoxes()
{
	_hitBox.actual.origin = ccpAdd(this->getPosition(), ccp(_hitBox.original.origin.x, _hitBox.original.origin.y));

	_attackBox.actual.origin = ccpAdd(this->getPosition(), ccp(_attackBox.original.origin.x +
					(this->getScaleX() == -1 ? (- _attackBox.original.size.width - _hitBox.original.size.width) : 0),
					_attackBox.original.origin.y));

	_circleAttackBox.actual.origin = ccpAdd(this->getPosition(), ccp(_circleAttackBox.original.origin.x ,
					_circleAttackBox.original.origin.y));

}


void ActionSprite::setPosition(CCPoint position)
{
	CCSprite::setPosition(position);
	this->transformBoxes();
}

