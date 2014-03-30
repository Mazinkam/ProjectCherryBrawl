/*
 * EnemyBossWings.h
 *
 *  Created on: 30.3.2014
 *      Author: user
 */

#ifndef ENEMYBOSSWINGS_H_
#define ENEMYBOSSWINGS_H_

#include "ActionSprite.h"

class EnemyBossWings: public ActionSprite
{
	public:
		EnemyBossWings();
		virtual ~EnemyBossWings();


	CREATE_FUNC (EnemyBossWings);

	bool init();
	void knockout();
};

#endif /* ENEMYBOSSWINGS_H_ */
