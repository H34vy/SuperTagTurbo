//=============================================================================
//	Object.h
//	Chris Prosser
//=============================================================================

#ifndef GameObject_H
#define GameObject_H

#include "d3dUtil.h"
#include "Geometry.h"
#include "constants.h"

class Object
{
public:

	//Constructors
	Object();
	~Object();	

	//Accessors
	Vector3 getPosition()						{return position;}
	Vector3 getVelocity()						{return velocity;}
	float getSpeed()							{return speed;}
	float getRadiusSquare()						{return radiusSquared;}
	float getRadius()							{return radius;}
	Matrix getWorldMatrix()						{return world;}
	Vector3 getScale()							{return Vector3(scaleX, scaleY, scaleZ);}
	bool isActive()								{return active;}
	bool isCollidable()							{return collidable;}

	//Mutators
	void setPosition (Vector3 pos)				{position = pos;}	
	void resetPosition()						{position = lastPosition; velocity = ZERO;}
	void setVelocity (Vector3 vel)				{velocity = vel;}	
	void setSpeed(float s)						{speed = s;}	
	void setRadius(float r)						{radius = r; radiusSquared = (scale*r)*(scale*r);}
	void setScale(float s)						{scale = s; radiusSquared = (s*radius)*(s*radius);}
	void setActive(bool a)						{active = a;}
	void setCollidable(bool c)					{collidable = c;}
	
	void setRotation(float x, float y, float z)	{rotX = x; rotY = y; rotZ = z; }	
	void setScale(float x, float y, float z)	{scaleX = x; scaleY = y; scaleZ = z; }

	//Polymorphism
	virtual void update(float dt);
	virtual void bounceCalc(Object* o){};
	virtual void bounce(Object* o){};
	virtual void bounce(){};	
	virtual void tag(){};
	
	//Other	
	void init(Geometry *g, Vector3 _pos, Matrix* view, Matrix* Projection, float _radius);	
	void draw(Technique* techDesc, MatrixVar* mVar);
	bool collided(Object *o);	
	bool rectCollided(Object *o);

protected:
	Geometry *geo;
	bool active, collidable;
	Vector3 position, lastPosition;
	Vector3 velocity;
	Matrix world, *mView, *mProj; //Pointers to the View and Projection matrices in the main app

	float speed, radius, radiusSquared, scale;
	float rotX, rotY, rotZ;
	float scaleX, scaleY, scaleZ;
};

#endif
