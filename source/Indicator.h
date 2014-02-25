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
	Indicator(int, Pyramid*, Input*, Object*);

	virtual void update(float dt);
	
protected:
	Input* input;
	
	int UP, DOWN, LEFT, RIGHT;
	CollisionType c;
	Pyramid* indicator;
	Object* player;

	//Vars used between bounce calculations
	float edgeTop, edgeBot, edgeLeft, edgeRight;
	float oEdgeTop, oEdgeBot, oEdgeLeft, oEdgeRight, oVelX, oVelY;

	
};

#endif