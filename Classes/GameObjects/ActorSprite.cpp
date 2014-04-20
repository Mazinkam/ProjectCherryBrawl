/*
 * ActorSprite.cpp
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#include "ActorSprite.h"
//#include "SimpleAudioEngine.h"

using namespace cocos2d;

ActorSprite::ActorSprite(void)
{

	//behaviour
	_idleAction = NULL;
	_walkAction = NULL;
	_walkIdle = NULL;
	_hurtAction = NULL;
	_knockedOutAction = NULL;
	_splitAttackDone = false;

	_attackDone = false;

	//attacks
	_attackAction = NULL;
	_circleAttackAction = NULL;
	_projectileAttackAction = NULL;
	_splittingAction = NULL;

}

ActorSprite::~ActorSprite(void)
{
}

void ActorSprite::idle()
{
	if (_actionState != kActionStateIdle)
	{
		this->stopAllActions();
		this->runAction(_idleAction);
		_actionState = kActionStateIdle;
		_velocity = CCPointZero; // same as 0,0
	}
}

void ActorSprite::pushBack()
{
	if (_actionState != kActionStateKnockedOut)
	{
		CCMoveBy *moveBy = CCMoveBy::create(1.0f, ccp(-2000, 0));
		this->runAction(moveBy);
		_actionState = kActionStateWalk;

	}
}


void ActorSprite::walkLeftThenIdle()
{
	if (_actionState == kActionStateIdle)
	{
		this->stopAllActions();

		CCMoveBy *moveBy = CCMoveBy::create(2.0f, (this->getScaleX() == -1 ? ccp(-200, 0): ccp(200, 0)));
		CCSpawn *spawn = CCSpawn::create(moveBy, _walkIdle, NULL);
		this->runAction(spawn);

		_actionState = kActionStateDialougeWalk;
	}

}
//add more states for different atks and sword movements
void ActorSprite::attack()
{
	_attackDone = true;
	if (_actionState == kActionStateIdle || _actionState != kActionStateAttack || _actionState == kActionStateWalk)
	{
		this->stopAllActions();
		this->runAction(_attackAction);
		_actionState = kActionStateAttack;
		_attackDone = false;
	}
}

void ActorSprite::circleAttack()
{
	if (_actionState == kActionStateIdle || _actionState != kActionStateAttack || _actionState == kActionStateWalk)
	{
		this->stopAllActions();
		this->runAction(_circleAttackAction);
		_actionState = kActionStateAttack;
	}
}

void ActorSprite::projectileAttack()
{
	if (_actionState == kActionStateIdle || _actionState != kActionStateAttack || _actionState == kActionStateWalk)
	{
		this->stopAllActions();
		this->runAction(_projectileAttackAction);
		_actionState = kActionStateAttack;
	}
}

void ActorSprite::hurtWithDamage(float damage)
{
	if (_actionState != kActionStateKnockedOut)
	{
		int randomSound = random_range(0, 1);
		//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("hit%d.wav", randomSound)->getCString());
		this->stopAllActions();
		this->runAction(_hurtAction);
		_actionState = kActionStateHurt;
		_hitPoints -= damage;

	}
}

void ActorSprite::knockout()
{
	this->stopAllActions();
	this->runAction(_knockedOutAction);
	_hitPoints = 0;
	_actionState = kActionStateKnockedOut; // k prefix is konstant!, _ for memeber variables
}

void ActorSprite::splitEnemy()
{
	if (_SpriteType == "Common")
	{
		this->stopAllActions();
		this->runAction(_splittingAction);
		_hitPoints = 0;
		_splitAttackDone = true;
		//_actionState = kActionStateKnockedOut; // k prefix is konstant!, _ for memeber variables
	} else
	{
		this->hurtWithDamage(50);
	}
}

void ActorSprite::walkWithDirection(CCPoint direction)
{
	if (_actionState == kActionStateIdle)
	{
		this->stopAllActions();
		if (getSpriteType() != "Wings")
		{
			this->runAction(_walkAction);
		} else
		{
			this->runAction(_idleAction);
		}
		_actionState = kActionStateWalk;
	}
	if (_actionState == kActionStateWalk || _actionState == kActionStateDialougeWalk)
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

void ActorSprite::update(float dt)
{
	if (_actionState == kActionStateWalk || _actionState == kActionStateDialougeWalk)
	{
		_desiredPosition = ccpAdd(this->getPosition(), ccpMult(_velocity, dt));
	}
}

BoundingBox ActorSprite::createBoundingBoxWithOrigin(CCPoint origin, CCSize size)
{
	BoundingBox boundingBox;
	boundingBox.original.origin = origin;
	boundingBox.original.size = size;
	boundingBox.actual.origin = ccpAdd(this->getPosition(), ccp(boundingBox.original.origin.x, boundingBox.original.origin.y));
	boundingBox.actual.size = size;
	return boundingBox;
}
//here for different attacks
void ActorSprite::transformBoxes()
{
	_hitBox.actual.origin = ccpAdd(this->getPosition(), ccp(_hitBox.original.origin.x, _hitBox.original.origin.y));

	_attackBox.actual.origin = ccpAdd(this->getPosition(), ccp(_attackBox.original.origin.x +
					(this->getScaleX() == -1 ? (- _attackBox.original.size.width - _hitBox.original.size.width) : 0),
					_attackBox.original.origin.y));

	_circleAttackBox.actual.origin = ccpAdd(this->getPosition(), ccp(_circleAttackBox.original.origin.x ,
					_circleAttackBox.original.origin.y));

	_splitAttackBox.actual.origin = ccpAdd(this->getPosition(), ccp(_splitAttackBox.original.origin.x ,
					_splitAttackBox.original.origin.y));

}

void ActorSprite::setPosition(CCPoint position)
{
	CCSprite::setPosition(position);
	this->transformBoxes();
}

void ActorSprite::cleanup()
{
//	CC_SAFE_RELEASE_NULL(_idleAction);
//	CC_SAFE_RELEASE_NULL(_attackAction);
//	CC_SAFE_RELEASE_NULL(_walkAction);
//	CC_SAFE_RELEASE_NULL(_walkIdle);
//	CC_SAFE_RELEASE_NULL(_hurtAction);
//	CC_SAFE_RELEASE_NULL(_knockedOutAction);
//	CCSprite::cleanup();
}
