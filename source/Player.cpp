//=============================================================================
//	Player.cpp
//	Chris Prosser
//=============================================================================

#include "Player.h"

const Vector3 VELOCITY_MAX(2,15,0);

Player::Player(int controlScheme, Box* t, Input* in)
{
	tagger = t;
	input = in;

	switch(controlScheme)
	{
	case 0: UP = 'W'; DOWN = 'S'; LEFT = 'A'; RIGHT = 'D'; break;
	case 1: UP = VK_UP; DOWN = VK_DOWN; LEFT = VK_LEFT; RIGHT = VK_RIGHT; break;
	}

	onPlatform = false;
	isTagger = false;	
}

void Player::update(float dt)
{	
	float xChange = 0, yChange = 0;

	//Decrease magnitude of velocity - friction, air resistance, etc.
	if (velocity.x > 0) xChange += -.025;
	if (velocity.x < 0) xChange += .025;
	if (velocity.y > 0) yChange += -.025;		

	//Jump
	if (input->isKeyDown(UP) && onPlatform) yChange += 25;

	//Lateral movement
	if (input->isKeyDown(LEFT)) xChange += -.05;	
	if (input->isKeyDown(RIGHT)) xChange += .05;

	//Increase fall speed
	if (input->isKeyDown(DOWN)) yChange += -.05;
	
	//Gravity
	if (!onPlatform) yChange -= .04;

	Vector3 accel = Vector3(xChange, yChange, 0); 	
	Vector3 absVel(abs(velocity.x), abs(velocity.y), abs(velocity.z));
	if (absVel < VELOCITY_MAX) velocity += accel;

	onPlatform = false;
	
	Object::update(dt);
}

void Player::bounceCalc(Object* o)
{
	edgeTop = position.y + scaleY;	
	edgeBot = position.y - scaleY;	
	oEdgeTop = o->getPosition().y + o->getScale().y;
	oEdgeBot = o->getPosition().y - o->getScale().y;	
	
	edgeLeft = position.x - scaleX;
	oEdgeLeft = o->getPosition().x - o->getScale().x;
	edgeRight = position.x + scaleX;
	oEdgeRight = o->getPosition().x + o->getScale().x;

	oVelX = o->getVelocity().x; oVelY = o->getVelocity().y;
}

void Player::bounce()
{

	float error = .05;
	//Player hitting right side of Platform
	if (abs(edgeLeft - oEdgeRight) < error)
	{
		position.x = oEdgeRight + scaleX + .0001;
		velocity.x *= -.25; velocity.x += .5*oVelX;
	}

	//Player hitting left side of Platform
	if (abs(edgeRight - oEdgeLeft) < error)
	{
		position.x = oEdgeLeft - scaleX - .0001;
		velocity.x *= -.25; velocity.x += .5*oVelX;
	}	

	//Player below platform
	if (abs(edgeTop - oEdgeBot) < error)
	{
		position.y = oEdgeBot - scaleY - .0001;
		velocity.y *= -.25; velocity.y += .25*oVelY;
	}

	//Player on platform
	if (abs(edgeBot - oEdgeTop) < error)
	{
		position.y = oEdgeTop + scaleY + .0001;
		velocity.y = 0;
		onPlatform = true;
	}	
}

void Player::tag()
{
	Box* temp = (Box*)geo;
	geo = tagger;
	tagger = temp;	

	isTagger = !isTagger;
}

