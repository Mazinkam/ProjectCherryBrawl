#ifndef  _DEFINES_
#define  _DEFINES_

#include "cocos2d.h"
#include <time.h> 

// 1 - convenience measurements
#define SCREEN CCDirector::sharedDirector()->getWinSize()
#define CENTER ccp(SCREEN.width / 2, SCREEN.height / 2)
#define CURTIME GetCurTime()
#define LOG cocos2d::CCLog

// 2 - convenience functions
#ifndef UINT64_C
	#define UINT64_C(val) val##ull
#endif
#define random_range(low, high) (rand() % (high - low + 1)) + low
#define frandom (float)rand() / UINT64_C(0x100000000)
#define frandom_range(low, high) ((high - low) * frandom) + low

//menu "magic nums"
static const int s_skillButtonSize = 76;
static const int s_betweenButtons = 60;
static const int s_sizeFromSide = 52;
static const int s_sidePadding = 20;
static const int s_horizPadding = 10;

static const int s_blipSize = 32;
static const int s_blipSpaceSize = 7;
static const int s_manaSize = 48;
static const int s_manaPadding = 15;
static const int s_manaSpaceSize = 15;
static const int s_lifeSize = 50;
static const int s_lifeSpaceSize = 10;

// 3 - enumerations
typedef enum _ActionState {
	kActionStateNone = 0,
	kActionStateIdle,
	kActionStateAttack,
	kActionStateWalk,
	kActionStateHurt,
	kActionStateKnockedOut,
	kActionStateDialougeWalk
} ActionState;

typedef enum _GameState {
	kGamePaused = 0,
	kGameRunning
} GameState;

// 4 - structures
typedef struct _BoundingBox {
	cocos2d::CCRect actual;
	cocos2d::CCRect original;
} BoundingBox;

inline float GetCurTime(){
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);

//	time_t now;
//	tm* local;
//	time(&now);
//	local=localtime(&now);
//	unsigned long millisecs = (local->tm_sec * 1000) + (local->tm_sec / 1000); // for visual studio
	return (float)millisecs;
};

//gets a random integer with a specified range
#define GET_RANDOM(min, max) \
((rand()%(int)(((max) + 1)-(min)))+ (min))

//adds child in the mentioned parent and positions it
#define ADDCHILD_POSITION(_object, _parent, _position){\
_parent->addChild(_object);\
_object->setPosition(_position);\
}

//adds child and positions it while giving it a tag and zindex
#define ADDCHILD_POSITION_TAG_ZINDEX(_object, _parent, _position, _tag, _zIndex){\
_parent->addChild(_object, _zIndex, _tag);\
_object->setPosition(_position);\
}

//safely releases and object
#define RELEASE_OBJECT(__POINTER) {\
	if(__POINTER){\
	__POINTER->release();\
	__POINTER = NULL;\
	}\
}

//safely releases an array
#define RELEASE_ARRAY(__POINTER) {\
	if(__POINTER){\
	__POINTER->removeAllObjects();\
	__POINTER->release();\
	__POINTER = NULL;\
	}\
}

//defines the fixtures
#define DEFINE_FIXTURE(_fixture,_shape,_density,_friction,_restitution){\
_fixture.shape = _shape; \
_fixture.density = _density; \
_fixture.friction = _friction; \
_fixture.restitution = _restitution; \
}

//defines the body
#define DEFINE_BODY(_bodyDef, _bodyType, _position, _userData) { \
	_bodyDef.type = _bodyType; \
	_bodyDef.position = Utility::toMeters(_position); \
	_bodyDef.userData = _userData; \
}


//helpers for getting content sizes
#define width(a) a->getContentSize().width
#define height(a) a->getContentSize().height

//helpers for getting postions
#define posy(a) a->getPosition().y
#define posx(a) a->getPosition().x

#endif
