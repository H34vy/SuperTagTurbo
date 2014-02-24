//=============================================================================
//	Indicator.cpp
//	David Goehring
//=============================================================================

#include "Indicator.h"

const Vector3 VELOCITY_MAX(2,15,0);

Indicator::Indicator(int controlScheme, Pyramid* t, Input* in, Object* p)
{
	player = p;
	input = in;
	indicator = t;
	
}

void Indicator::update(float dt)
{	
	position.x = player->getPosition().x;
	position.y = player->getPosition().y+3;
	position.z = player->getPosition().z;
	
	Object::update(dt);
}
