//=============================================================================
//	Tag.cpp
//	Chris Prosser
//		Modified from: 
//		Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=============================================================================

#include "d3dApp.h"
#include <d3dx9math.h>
#include "Tag.h"
#include "Random.h"

Random rGen(0,1);

Tag::Tag(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);	 
}

Tag::~Tag()
{
	if(md3dDevice) md3dDevice->ClearState();	

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

	for (int i=0; i<OBJECT_COUNT; i++) delete objects[i];
}

void Tag::initApp()
{
	D3DApp::initApp();
	
	gameState = 0;

	menu_Title = "Super Tag Turbo!";
	game_Seconds = 120;
	game_Minutes = (game_Seconds/60);

	platformBox.init(md3dDevice, 1.0f, BLACK);
	playerBox.init(md3dDevice, 1.0f, DARKBROWN);
	wallBox.init(md3dDevice, 1.0f);
	taggerBox.init(md3dDevice, 1.0f, RED);

	lineR.init(md3dDevice, 10.0f, RED);	
	lineW.init(md3dDevice, 10.0f, GREEN);
	lineB.init(md3dDevice, 10.0f, BLUE);

	for (int i=0; i<OBJECT_COUNT; i++) objects[i] = new Object();

	/*objects[X_AXIS]->init(&lineR, Vector3(-10,0,0), &mView, &mProj, 5);
	objects[Y_AXIS]->init(&lineW, Vector3(0,-2.5,0), &mView, &mProj, 5); 
	objects[Z_AXIS]->init(&lineB, Vector3(0,0,0), &mView, &mProj, 5);

	objects[X_AXIS]->setScale(2,1,1);	

	objects[Y_AXIS]->setRotation(0,0, ToRadian(90));
	objects[Z_AXIS]->setRotation(0, ToRadian(90), 0);*/

	const int Z = -1;

	objects[BOT_LEFT_PLATFORM]->init(&platformBox, Vector3(-7, 2, Z), &mView, &mProj, .7071);
	objects[BOT_RIGHT_PLATFORM]->init(&platformBox, Vector3(7, 2, Z), &mView, &mProj, .7071);
	objects[TOP_LEFT_PLATFORM]->init(&platformBox, Vector3(-10, 12, Z), &mView, &mProj, .7071);
	objects[TOP_RIGHT_PLATFORM]->init(&platformBox, Vector3(10, 12, Z), &mView, &mProj, .7071);
	objects[MIDDLE_PLATFORM]->init(&platformBox, Vector3(0, 6.5, Z), &mView, &mProj, .7071);

	objects[BOT_LEFT_PLATFORM]->setScale(4,.25,1);
	objects[BOT_RIGHT_PLATFORM]->setScale(4,.25,1);
	objects[TOP_LEFT_PLATFORM]->setScale(2,.25,1);
	objects[TOP_RIGHT_PLATFORM]->setScale(2,.25,1);
	objects[MIDDLE_PLATFORM]->setScale(3,.25,1);

	objects[BACK_WALL]->init(&wallBox, Vector3(0,20,-Z), &mView, &mProj, .7071);
	objects[LEFT_WALL]->init(&wallBox, Vector3(-16,20,Z), &mView, &mProj, .7071);
	objects[RIGHT_WALL]->init(&wallBox, Vector3(16,20,Z), &mView, &mProj, .7071);

	//objects[TOP_WALL]->init(&wallBox, Vector3(0,20,Z), &mView, &mProj, sqrt(2));
	objects[BOTTOM_WALL]->init(&wallBox, Vector3(0,-4,Z), &mView, &mProj, .7071);

	objects[BACK_WALL]->setScale(16,25, 1);
	objects[LEFT_WALL]->setScale(1,25, -2*Z);
	objects[RIGHT_WALL]->setScale(1,25, -2*Z);	
	//objects[TOP_WALL]->setScale(15,1, -2*Z);
	objects[BOTTOM_WALL]->setScale(15,1, -2*Z);
	
	delete objects[PLAYER1]; objects[PLAYER1] = new Player(0, &taggerBox, input);
	delete objects[PLAYER2]; objects[PLAYER2] = new Player(1, &taggerBox, input);

	objects[PLAYER1]->init(&playerBox, Vector3(-13, 0, Z), &mView, &mProj, .7071);
	objects[PLAYER2]->init(&playerBox, Vector3(13, 0, Z), &mView, &mProj, .7071);
	
	if (rGen.next()) objects[PLAYER1]->tag();
	else objects[PLAYER2]->tag();

	objects[BACK_WALL]->setCollidable(false);

	buildFX();
	buildVertexLayouts();
}

void Tag::updateScene(float dt)
{
	if(gameState == 0){
		D3DApp::updateScene(dt);
		if(input->isKeyDown(VK_RETURN)){
			gameState = 1;
			mTimer.reset();
		}
	}
	else if(gameState == 1){
		D3DApp::updateScene(dt);
		for (int i=0; i<OBJECT_COUNT; i++) objects[i]->update(dt);	
		collisions();	
	}

	// Build the view matrix.
	Vector3 pos(0.f,8.f,-37.0f);
	Vector3 target(0.0f, 8.f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);
	LookAt(&mView, &pos, &target, &up);
}

void Tag::collisions()
{

	//Wholistic appropach
	/*for (int i=0; i<OBJECT_COUNT; i++)	
		for (int j=i+1; j<OBJECT_COUNT; j++)		
			if (objects[i]->rectCollided(objects[j]))
			{
				objects[i]->resetPosition();
				objects[j]->resetPosition();
			}*/
	
	//More efficient approach for Tag
	for (int i=0; i<OBJECT_COUNT; i++)	
		if (i != PLAYER1 && i != PLAYER2 )
		{
			if (objects[PLAYER1]->rectCollided(objects[i])) 
				objects[PLAYER1]->bounce(objects[i]);

			if (objects[PLAYER2]->rectCollided(objects[i])) 
				objects[PLAYER2]->bounce(objects[i]);			
		}	

	if (objects[PLAYER1]->rectCollided(objects[PLAYER2]))
	{
		objects[PLAYER1]->bounceCalc(objects[PLAYER2]);
		objects[PLAYER2]->bounceCalc(objects[PLAYER1]);
		objects[PLAYER1]->bounce();
		objects[PLAYER2]->bounce();
		objects[PLAYER1]->tag();
		objects[PLAYER2]->tag();
	}	
	
}

void Tag::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);

// Main Menu
	if(gameState == 0){
		RECT title = {0, 0, 200, 200};
		mFont->DrawTextA(NULL, menu_Title.c_str(), 16, &title, DT_VCENTER, RED);
	}

// Game is underway
	else if(gameState == 1){
		//draw objects	
		for (int i=0; i<OBJECT_COUNT; i++) objects[i]->draw(mTech, mfxWVPVar);
		RECT title = {0, 0, 200, 100};
		std::stringstream s;
// Game Clock *WORK IN PROGRESS*
		int seconds = game_Seconds - int(mTimer.getGameTime());
		int minutes = seconds/60;
		if(seconds%60 >= 10){
			s << minutes << ":" << seconds%60;
		}
		else if(seconds%60 < 10 && seconds%60 >= 0){
			s << minutes << ":" << "0" << seconds%60;
		}
		else if(seconds < 0){
			seconds = 0;
			minutes = 0;
			s << minutes << ":" << seconds%60;
		}

		std::string ws = s.str();
		mFont->DrawTextA(NULL, ws.c_str(), -1, &title, DT_VCENTER, BLACK);
	}


	//draw font
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	//RECT R = {5, 5, 0, 0};
	//mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	
	mSwapChain->Present(0, 0);
}


//Don't touch these!
void Tag::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"source//color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxFLIPVar = mFX->GetVariableByName("flip");

}

void Tag::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void Tag::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*3.14159, aspect, 1.0f, 1000.0f);
}