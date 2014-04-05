/*
 * EnemyFemale.h
 *
 *  Created on: 9.3.2014
 *      Author: user
 */

#ifndef ENEMYFEMALE_H_
#define ENEMYFEMALE_H_

#include "ActorSprite.h"

class EnemyFemale: public ActionSprite
{
	public:
		EnemyFemale(void);
		virtual ~EnemyFemale(void);

		CREATE_FUNC (EnemyFemale);

		bool init();
		void knockout();

		CC_SYNTHESIZE(float, _nextDecisionTime, NextDecisionTime);
};
#endif /* ENEMYFEMALE_H_ */
