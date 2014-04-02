/*
 * ObjectShadow.h
 *
 *  Created on: 2.4.2014
 *      Author: user
 */

#ifndef OBJECTSHADOW_H_
#define OBJECTSHADOW_H_

#include "ActionSprite.h"

class ObjectShadow: public ActionSprite
{
	public:
		ObjectShadow();
		virtual ~ObjectShadow();


	CREATE_FUNC (ObjectShadow);

	bool init();
	void knockout();
};

#endif /* OBJECTSHADOW_H_ */
