//=============================================================================
//	Indicator.cpp
//	David Goehring
//=============================================================================

#include "Indicator.h"

Indicator::Indicator(Pyramid* t, Object* p1, Object* p2)
{
	indicator = t;
	player1 = p1;
	player2 = p2;
}

void Indicator::update(float dt)
{
	Object* tagger;
	active = true;
	if (player1->get_Tagger()) tagger = player1;
	else if (player2->get_Tagger()) tagger = player2;
	else {active = false; return; }
	position.x = tagger->getPosition().x;
	position.y = tagger->getPosition().y+3;
	position.z = tagger->getPosition().z;
	
	Object::update(dt);
}