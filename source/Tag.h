//=============================================================================
//	Tag.h
//	Chris Prosser
//		Modified from: 
//		Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=============================================================================
#ifndef TAG_H
#define TAG_H

#include "d3dApp.h"
#include "audio.h"
#include "Geometry.h"
#include "Input.h"
#include "Object.h"
#include "Player.h"
#include "Indicator.h"

const int Z = -1;

//Object indicies 
enum
{
	//X_AXIS=0,
	//Y_AXIS,
	//Z_AXIS,

	BOT_LEFT_PLATFORM=0,
	BOT_RIGHT_PLATFORM,
	MIDDLE_PLATFORM,
	TOP_LEFT_PLATFORM,
	TOP_RIGHT_PLATFORM,	

	PLAYER1,
	PLAYER2,	

	INDICATOR,

	BACK_WALL,
	LEFT_WALL,
	RIGHT_WALL,
	BOTTOM_WALL,

	//Accurate if last
	OBJECT_COUNT
};

class Tag : public D3DApp
{
public:
	Tag(HINSTANCE hInstance);
	~Tag();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void collisions();
	void drawScene(); 
	Vector3 moveCube();

	int gameState;
	std::string menu_Title;
	std::string clock_Text;
	int game_Minutes;
	float game_Seconds;
	float player1_Score;
	float player2_Score;
	float last_frame_Time;
	float current_frame_Time;
	float lastFrameSeconds;

private:
	void buildFX();
	void buildVertexLayouts();
	void reset();

	//Geometries
	Line lineR, lineW, lineB;
	Box platformBox, player1Box, player2Box;
	RainbowBox wallBox;
	Pyramid indicator;

	int P1wins, P2wins;

	//Array of objects	
	Object* objects[OBJECT_COUNT];	

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectVariable* mfxFLIPVar;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	
	Matrix mView;
	Matrix mProj;
	Audio* audio;
	HRESULT hr;
};

#endif