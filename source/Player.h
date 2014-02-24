//=============================================================================
//	Player.h
//	Chris Prosser
//=============================================================================

#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Input.h"

enum CollisionType{HOR, VERT};

class Player: public Object
{
public:
	Player(int, Box*, Input*);

	virtual void update(float dt);
	virtual void tag();	
	virtual void bounceCalc(Object* o);
	virtual void bounce();
	virtual void bounce(Object* o)			{bounceCalc(o); bounce();}
	
protected:
	Input* input;
	
	int UP, DOWN, LEFT, RIGHT;
	CollisionType c;
	bool onPlatform, isTagger;
	Box* tagger;

	//Vars used between bounce calculations
	float edgeTop, edgeBot, edgeLeft, edgeRight;
	float oEdgeTop, oEdgeBot, oEdgeLeft, oEdgeRight, oVelX, oVelY;

	
};

#endif