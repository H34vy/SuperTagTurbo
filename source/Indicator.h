//=============================================================================
// Indicator.h
// David Goehring
//=============================================================================

#ifndef INDICATOR_H
#define INDICATOR_H

#include "Object.h"
#include "Input.h"
#include "Player.h"


class Indicator: public Object
{
public:
	Indicator(Pyramid*, Object*, Object*);

	virtual void update(float dt);
	
protected:
	CollisionType c;
	Pyramid* indicator;
	Object* player1;
	Object* player2;

	//Vars used between bounce calculations
	float edgeTop, edgeBot, edgeLeft, edgeRight;
	float oEdgeTop, oEdgeBot, oEdgeLeft, oEdgeRight, oVelX, oVelY;
	
};

#endif