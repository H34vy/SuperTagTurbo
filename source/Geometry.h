//=============================================================================
//	Geometry.h
//	Chris Prosser
//=============================================================================

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "d3dUtil.h"
#include "constants.h"

class Geometry{
public:

	Geometry();
	~Geometry();

	virtual void init(Device* d, float scale, Color c = BLACK){};
	virtual void draw(){};
	bool isCollidable(){return collidable;}

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	bool collidable;
	Device* device;
	Buffer* mVB;
	Buffer* mIB;
};

//All lengths default to 1 or -1, where applicable

class Box: public Geometry{
public:	
	virtual void init(Device* d, float scale, Color c = BLACK);
	virtual void draw();
};

class RainbowBox: public Geometry{
public:	
	virtual void init(Device* d, float scale);
	virtual void draw();
};

class Line: public Geometry{
public:	
	virtual void init(Device* d, float scale, Color c = BLACK);
	virtual void draw();
};

class Pyramid: public Geometry {
public:
	virtual void init(Device* d, float scale, Color c = BLACK);
	virtual void draw();
};

#endif