#include "SimpleDPad.h"
#include "../GameResources.h"
#include <math.h>

using namespace cocos2d;
using namespace std;

SimpleDPad::SimpleDPad(void)
{
	_delegate = NULL;
	_touchStick = NULL;
}

SimpleDPad::~SimpleDPad(void)
{
}

SimpleDPad* SimpleDPad::dPadWithFile(CCString *fileName, float radius)
{
	SimpleDPad *pRet = new SimpleDPad();
	if (pRet && pRet->initWithFile(fileName, radius))
	{
		return pRet;
	} else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool SimpleDPad::initWithFile(CCString *filename, float radius)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithFile(filename->getCString()));

		_radius = radius;
		_direction = CCPointZero;
		_isHeld = false;
		_touchStick = CCSprite::create(s_DpadBtn);
		CC_BREAK_IF(!_touchStick);

		_touchStick->setPosition(ccp(75.0,75.0));
		_touchStick->setAnchorPoint(ccp(this->getAnchorPoint().x,this->getAnchorPoint().y));

		this->scheduleUpdate();
		this->addChild(_touchStick);
		bRet = true;
	} while (0);

	return bRet;
}

void SimpleDPad::onEnterTransitionDidFinish()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void SimpleDPad::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void SimpleDPad::update(float dt)
{
	if (_isHeld)
	{
		_delegate->isHoldingDirection(this, _direction);
	}
}

bool SimpleDPad::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocation();

	float distanceSQ = ccpDistanceSQ(location, this->getPosition());
	if (distanceSQ <= _radius * _radius) // && location.x >= 14 && location.x <= 130 && location.y >= 8 && location.y <= 130)
	{
		this->updateDirectionForTouchLocation(location);
		//cocos2d::CCLog("location x: %lf  -- location y: %lf ", location.y, location.x);
		_isHeld = true;
		return true;
	}
	return false;
}

void SimpleDPad::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocation();

	this->updateDirectionForTouchLocation(location);
}

void SimpleDPad::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	_direction = CCPointZero;
	_isHeld = false;
	_touchStick->setPosition(ccp(75.0, 75.0));
	_delegate->simpleDPadTouchEnded(this);
}

void SimpleDPad::updateDirectionForTouchLocation(CCPoint location)
{
	float radians = ccpToAngle(ccpSub(location, this->getPosition()));
	float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);

	if (location.x >= 25 && location.x <= 120 && location.y >= 16 && location.y <= 120)
	{

		_touchStick->setPosition(ccp(location.x, location.y));

	} else
	{
		double _circleX = location.x - (this->getPositionX()-20);
		double _circleY = location.y - (this->getPositionY()-20);

		float _circleD = sqrt(pow(_circleX, 2.0) + pow(_circleY, 2.0));
		float _newPointX = (this->getPositionX()-20)+ (location.x - (this->getPositionX()-20)) * 60.0f / _circleD;
		float _newPointY = (this->getPositionY()-20)+ (location.y - (this->getPositionY()-20)) * 60.0f / _circleD;

		_touchStick->setPosition(ccp(_newPointX, _newPointY));

//		cocos2d::CCLog("location.x: %lf  -- location.y: %lf --  point.x: %lf --  point.y: %lf", location.x, location.y, xp , yp);

	}
//	cocos2d::CCLog("location.x: %lf  -- location.y: %lf ", location.x, location.y);

	if (degrees <= 22.5 && degrees >= -22.5)
	{
		//right
		_direction = ccp(1.0, 0.0);
	}
	else if (degrees > 22.5 && degrees < 67.5)
	{
		//bottom right
				_direction = ccp(1.0, -1.0);
			}
			else if (degrees >= 67.5 && degrees <= 112.5)
			{
				//bottom
				_direction = ccp(0.0, -1.0);
			}
			else if (degrees > 112.5 && degrees < 157.5)
			{
				//bottom left
				_direction = ccp(-1.0, -1.0);
			}
			else if (degrees >= 157.5 || degrees <= -157.5)
			{
				//left
				_direction = ccp(-1.0, 0.0);
			}
			else if (degrees < -22.5 && degrees > -67.5)
			{
				//top right
				_direction = ccp(1.0, 1.0);
			}
			else if (degrees <= -67.5 && degrees >= -112.5)
			{
				//top
				_direction = ccp(0.0, 1.0);
			}
			else if (degrees < -112.5 && degrees > -157.5)
			{
				//top left
				_direction = ccp(-1.0, 1.0);
			}

	_delegate->didChangeDirectionTo(this, _direction);
}
