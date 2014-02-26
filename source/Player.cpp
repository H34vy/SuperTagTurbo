//=============================================================================
//	Player.cpp
//	Chris Prosser
//=============================================================================

#include "Player.h"

const float JUMP_SPEED(26.f);
const float GRAVITY(75.f);

Player::Player(int controlScheme, Input* in, Audio* aud)
{
	input = in;
	audio = aud;
	switch(controlScheme)
	{
	case 0: UP = 'W'; DOWN = 'S'; LEFT = 'A'; RIGHT = 'D'; break;
	case 1: UP = VK_UP; DOWN = VK_DOWN; LEFT = VK_LEFT; RIGHT = VK_RIGHT; break;
	}

	onPlatform = false;
	isTagger = false;	

	jumpLeft = 0;
}

void Player::update(float dt)
{	
	float xChange = 0, yChange = 0;	

	//Jump
	if (input->isKeyDown(UP) && onPlatform) {
		jumpLeft = 2;
		audio->playCue(HIT);
	}
	//Lateral movement
	if (input->isKeyDown(LEFT)) xChange += -65;	
	if (input->isKeyDown(RIGHT)) xChange += 65;

	//Increase fall speed
	//if (input->isKeyDown(DOWN)) yChange += -15;	
	
	//Decreased midair movement
	if (!onPlatform){
		if (velocity.x > 0) xChange += -15;
		if (velocity.x < 0) xChange += 15;
		xChange *=.5;	
	}
	else
	{
		//Friction/air resistance
		if (velocity.x > 0) xChange += -30;
		if (velocity.x < 0) xChange += 30;
		if (velocity.y > 0) yChange += -15;	
	}

	if (jumpLeft > 0)
	{
		velocity.y = JUMP_SPEED;
		jumpLeft -= velocity.y*dt;
	}

	velocity.y -= GRAVITY*dt;
	velocity.x += xChange*dt;

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
	float error = .06;

	//Player on Object
	if (abs(edgeBot - oEdgeTop) < error)
	{
		position.y = oEdgeTop + scaleY;
		velocity.y = 0;
		onPlatform = true;
		jumpLeft = -1;
		return;
	}	

	//Player hitting right side of Object
	if (abs(edgeLeft - oEdgeRight) < error)
	{
		position.x = oEdgeRight + scaleX + .001;
		velocity.x *= -.25; velocity.x += .5*oVelX;
	}

	//Player hitting left side of Object
	if (abs(edgeRight - oEdgeLeft) < error)
	{
		position.x = oEdgeLeft - scaleX - .001;
		velocity.x *= -.25; velocity.x += .5*oVelX;
	}	

	//Player below Object
	if (abs(edgeTop - oEdgeBot) < error)
	{
		position.y = oEdgeBot - scaleY;
		velocity.y *= -.25; velocity.y += .25*oVelY;
	}

	//Special wall checks
	if (edgeBot < -3.f)
	{
		position.y = -3.f + scaleY;
		velocity.y = 0;
	}

	if (edgeLeft < -15.f) position.x = -15.f + scaleX;
	if (edgeRight > 15.f) position.x = 15.f - scaleX;	
}

void Player::tag()
{
	isTagger = !isTagger;
}

