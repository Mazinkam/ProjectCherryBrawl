/*
 * Cherry.h
 *
 *  Created on: 3.3.2014
 *      Author: user
 */

#ifndef CHERRY_H_
#define CHERRY_H_

#include "ActionSprite.h"

class Cherry : public ActionSprite
{
public:
	Cherry(void);
	virtual ~Cherry(void);

	CREATE_FUNC(Cherry);
	bool init();
	void knockout();

};


#endif /* CHERRY_H_ */
