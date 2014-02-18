//=============================================================================
//	WinMain.cpp
//	Chris Prosser
//	Modified from:
//		Programming 2D Games
//		Copyright (c) 2011 by:
//		Charles Kelly
//		CollisionTypes winmain.cpp v1.0
//=============================================================================

#include "d3dApp.h"
#include <d3dx9math.h>
#include "Tag.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Tag game(hInstance);
	game.initApp();
	return game.run();	
}