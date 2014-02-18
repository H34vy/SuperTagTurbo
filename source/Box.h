//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef BOX_H
#define BOX_H

#include "d3dUtil.h"
#include "constants.h"

class Box
{
public:

	Box();
	~Box();

	void init(Device* device, float scale, Color c = BLACK);
	void draw();

private:
	DWORD mNumVertices;
	DWORD mNumFaces;

	Device* md3dDevice;
	Buffer* mVB;
	Buffer* mIB;
};


#endif // BOX_H
