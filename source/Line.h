#ifndef LINE_H
#define LINE_H

#include "d3dUtil.h"
#include "constants.h"

class Line
{
public:

	Line() : mNumVertices(0), mNumFaces(0), device(0), mVB(0), mIB(0){};
	~Line(){ ReleaseCOM(mVB); ReleaseCOM(mIB); }

	//Accessors

	//Return World Matrix to rotate Line onto Y-axis
	static Matrix getMatY(){ Matrix mWorld; Identity(&mWorld); RotateZ(&mWorld, ToRadian(90)); return mWorld; }
	//Return World Matrix to rotate Line onto Z-axis
	static Matrix getMatZ(){ Matrix mWorld; Identity(&mWorld); RotateY(&mWorld, ToRadian(90)); return mWorld; }

	//Workers
	virtual void init(Device* device, float scale, Color color = BLACK);
	virtual void draw();

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;

	Device* device;
	Buffer* mVB;
	Buffer* mIB;
};

#endif