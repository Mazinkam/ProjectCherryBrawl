/*
 * ActionSprite.h
 *
 *  Created on: 24.2.2014
 *      Author: user
 */

#ifndef ACTIONSPRITE_H_
#define ACTIONSPRITE_H_

#include "cocos2d.h"
#include "../Defines.h"
#include "../GameResources.h"

class ActionSprite: public cocos2d::CCSprite
{
	public:
		ActionSprite(void);
		virtual ~ActionSprite(void);

		//action methods
		void idle();
		void hurtWithDamage(float damage);
		virtual void knockout();
		void walkWithDirection(cocos2d::CCPoint direction);

		void attack();
		void circleAttack();
		void projectileAttack();


		//scheduled methods
		void update(float dt);

		BoundingBox createBoundingBoxWithOrigin(cocos2d::CCPoint origin, cocos2d::CCSize size);
		void transformBoxes();
		void setPosition(cocos2d::CCPoint position);

		//actions
		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _idleAction, IdleAction);
		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _walkAction, WalkAction);
		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _hurtAction, HurtAction);
		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _knockedOutAction, KnockedOutAction);

		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _attackAction, AttackAction);
		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _circleAttackAction, CircleAttackAction);
		CC_SYNTHESIZE_RETAIN(cocos2d::CCAction*, _projectileAttackAction, ProjectileAttackAction);

		//states
		CC_SYNTHESIZE(ActionState, _actionState, ActionState);

		//attributes
		CC_SYNTHESIZE(float, _walkSpeed, WalkSpeed);
		CC_SYNTHESIZE(float, _hitPoints, HitPoints);
		CC_SYNTHESIZE(float, _life, Life);
		CC_SYNTHESIZE(float, _damage, Damage); //needs different damages for differnet skills
		CC_SYNTHESIZE(float, _projectileDamage, ProjectileDamage); //needs different damages for differnet skills
		CC_SYNTHESIZE(float, _circleDamage, CircleDamage); //needs different damages for differnet skills
		CC_SYNTHESIZE(float, _manaPool, ManaPool);

		//movement
		CC_SYNTHESIZE(cocos2d::CCPoint, _velocity, Velocity);
		CC_SYNTHESIZE(cocos2d::CCPoint, _desiredPosition, DesiredPosition);

		//measurements
		CC_SYNTHESIZE(float, _centerToSides, CenterToSides);
		CC_SYNTHESIZE(float, _centerToBottom, CenterToBottom);

		CC_SYNTHESIZE(BoundingBox, _hitBox, Hitbox);

		//attacks
		CC_SYNTHESIZE(BoundingBox, _attackBox, AttackBox);
		CC_SYNTHESIZE(BoundingBox, _circleAttackBox, CircleAttackBox);
		CC_SYNTHESIZE(BoundingBox, _projectileAttackBox, ProjectileAttackBox);


};

#endif /* ACTIONSPRITE_H_ */
