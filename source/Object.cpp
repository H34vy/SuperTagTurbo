//=============================================================================
//	Object.cpp
//	Chris Prosser
//=============================================================================

#include "Object.h"

Object::Object()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
}

Object::~Object()
{
	geo = NULL;
}

void Object::draw(Technique* mTech, MatrixVar* mVar)
{
	if (!active) return;

	Matrix mWVP = world * (*mView) * (*mProj);
	mVar->SetMatrix((float*)&mWVP);

    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc(&techDesc);
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex(p)->Apply(0);
        geo->draw();
    }
}

void Object::init(Geometry *g, Vector3 pos, Matrix* view, Matrix* Projection, float _radius)
{
	geo = g; 
	collidable = geo->isCollidable();
	position = pos;	
	mView = view;
	mProj = Projection;
	radius = _radius * 1.01;		
	radiusSquared = radius * radius;	
	velocity = Vector3(0,0,0);
	rotX = rotY = rotZ = 0;	
	scaleX = scaleY = scaleZ = 1;
}

void Object::update(float dt)
{			
	Matrix rotXM, rotYM, rotZM, scaleM, transM;	

	Identity(&rotXM); RotateX(&rotXM, rotX);
	Identity(&rotYM); RotateY(&rotYM, rotY);
	Identity(&rotZM); RotateZ(&rotZM, rotZ); 

	Identity(&scaleM); Scale(&scaleM, scaleX, scaleY, scaleZ);

	lastPosition = position;
	position += velocity*dt;
	Identity(&world);
	Translate(&transM, position.x, position.y, position.z);	

	world = rotXM * rotYM * rotZM * scaleM * transM;
}

bool Object::collided(Object *o)
{
	if (!collidable || !o->isActive() || !o->isCollidable()) return false;
	Vector3 diff = position - o->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + o->getRadiusSquare();
	if (length <= radii) return true;
	return false;
}

bool Object::rectCollided(Object *o)
{
	if (!collidable || !o->isActive() || !o->isCollidable()) return false;
	Vector3 oPos = o->getPosition(), oScale = o->getScale();

	if (position.x + scaleX > oPos.x - oScale.x && 
		position.x - scaleX < oPos.x + oScale.x &&
		position.y + scaleY > oPos.y - oScale.y && 
		position.y - scaleY < oPos.y + oScale.y) 
		return true;	

	return false;
}