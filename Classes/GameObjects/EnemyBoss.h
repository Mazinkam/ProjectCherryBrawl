/*
 * EnemyBoss.h
 *
 *  Created on: 19.3.2014
 *      Author: user
 */

#ifndef ENEMYBOSS_H_
#define ENEMYBOSS_H_

#include "ActorSprite.h"

class EnemyBoss: public ActionSprite
{
	public:
	EnemyBoss(void);
	virtual ~EnemyBoss(void);

	CREATE_FUNC (EnemyBoss);

	bool init();
	void knockout();

	CC_SYNTHESIZE(float, _nextDecisionTime, NextDecisionTime);
	CC_SYNTHESIZE(bool, _animDone, AnimDone);
};

#endif /* ENEMYBOSS_H_ */
