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
	
	audio = new Audio();
	if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')  // if sound files defined
    {
        if( FAILED( hr = audio->initialize() ) )
        {
            if( hr == HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) )
                throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system because media file not found."));
            else
                throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system."));
        }
    }
	audio->run();


	platformBox.init(md3dDevice, 1.0f, DARKBROWN);
	player1Box.init(md3dDevice, 1.0f, WHITE);
	wallBox.init(md3dDevice, 1.0f);
	player2Box.init(md3dDevice, 1.0f, BLACK);

	indicator.init(md3dDevice, 0.5f, RED);
	
	for (int i=0; i<OBJECT_COUNT; i++) objects[i] = new Object();

	/*objects[X_AXIS]->init(&lineR, Vector3(-10,0,0), &mView, &mProj, 5);
	objects[Y_AXIS]->init(&lineW, Vector3(0,-2.5,0), &mView, &mProj, 5); 
	objects[Z_AXIS]->init(&lineB, Vector3(0,0,0), &mView, &mProj, 5);

	objects[X_AXIS]->setScale(2,1,1);	

	objects[Y_AXIS]->setRotation(0,0, ToRadian(90));
	objects[Z_AXIS]->setRotation(0, ToRadian(90), 0);*/

	objects[BOT_LEFT_PLATFORM]->setScale(4,.25,1);
	objects[BOT_RIGHT_PLATFORM]->setScale(4,.25,1);
	objects[TOP_LEFT_PLATFORM]->setScale(2,.25,1);
	objects[TOP_RIGHT_PLATFORM]->setScale(2,.25,1);
	objects[MIDDLE_PLATFORM]->setScale(3,.25,1);
	
	objects[BOT_LEFT_PLATFORM]->init(&platformBox, Vector3(-7, 2, Z), &mView, &mProj, .7071);
	objects[BOT_RIGHT_PLATFORM]->init(&platformBox, Vector3(7, 2, Z), &mView, &mProj, .7071);
	objects[TOP_LEFT_PLATFORM]->init(&platformBox, Vector3(-10, 12, Z), &mView, &mProj, .7071);
	objects[TOP_RIGHT_PLATFORM]->init(&platformBox, Vector3(10, 12, Z), &mView, &mProj, .7071);
	objects[MIDDLE_PLATFORM]->init(&platformBox, Vector3(0, 6.5, Z), &mView, &mProj, .7071);
	
	objects[BACK_WALL]->setScale(16,25, 1);
	objects[LEFT_WALL]->setScale(1,25, -2*Z);
	objects[RIGHT_WALL]->setScale(1,25, -2*Z);	
	
	objects[BACK_WALL]->init(&wallBox, Vector3(0,20,-Z), &mView, &mProj, .7071);
	objects[LEFT_WALL]->init(&wallBox, Vector3(-16,20,Z), &mView, &mProj, .7071);
	objects[RIGHT_WALL]->init(&wallBox, Vector3(16,20,Z), &mView, &mProj, .7071);
	
	objects[BOTTOM_WALL]->setScale(15,1, -2*Z);
	objects[BOTTOM_WALL]->init(&wallBox, Vector3(0,-4,Z), &mView, &mProj, .7071);
	
	delete objects[PLAYER1]; objects[PLAYER1] = new Player(0, input, audio);
	delete objects[PLAYER2]; objects[PLAYER2] = new Player(1, input, audio);

	objects[PLAYER1]->init(&player1Box, Vector3(-13,0,Z), &mView, &mProj, .7071);
	objects[PLAYER2]->init(&player2Box, Vector3(13,0, Z), &mView, &mProj, .7071);
	
	delete objects[INDICATOR]; objects[INDICATOR] = new Indicator(&indicator, objects[PLAYER1], objects[PLAYER2]);	
	objects[INDICATOR]->init(&indicator, Vector3(100,100,100), &mView, &mProj, .7071);
	
	if (rGen.next()) objects[PLAYER1]->tag();
	else objects[PLAYER2]->tag();

	objects[BACK_WALL]->setCollidable(false);

	gameState = 0;
	std::string tagger="";
	if (objects[PLAYER1]->get_Tagger()) tagger = "White";
	if (objects[PLAYER2]->get_Tagger()) tagger = "Black";
	
	menu_Title = "SuperTagTurbo!\n"+tagger+" is \"It!\"\nPress any key to begin";
	game_Seconds = 60;
	game_Minutes = (game_Seconds/60);
	player1_Score = 0;
	player2_Score = 0;
	last_frame_Time = 0;
	current_frame_Time = 0;
	P1wins = 0;
	P2wins = 0;

	buildFX();
	buildVertexLayouts();
	
	audio->playCue(BG);
}

void Tag::updateScene(float dt)
{
	current_frame_Time = mTimer.getGameTime();
	if(gameState == 0){
		D3DApp::updateScene(dt);
		if(input->anyKeyPressed()){
			gameState = 1;
			mTimer.reset();
		}
	}
	else if(gameState == 1){
		if (current_frame_Time >= 60) reset();
				
		D3DApp::updateScene(dt);
		for (int i=0; i<OBJECT_COUNT; i++) objects[i]->update(dt);	
		collisions();	

		if(objects[PLAYER1]->get_Tagger()){
			player1_Score += current_frame_Time - last_frame_Time; 
		}
		else if(objects[PLAYER2]->get_Tagger()){
			player2_Score += current_frame_Time - last_frame_Time;
		}
	}

	// Build the view matrix.
	Vector3 pos(0.f,8.f,-37.0f);
	Vector3 target(0.0f, 8.f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);
	LookAt(&mView, &pos, &target, &up);
}

void Tag::collisions()
{	
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
		audio->playCue(TAG);
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

	//draw objects	
	for (int i=0; i<OBJECT_COUNT; i++) objects[i]->draw(mTech, mfxWVPVar);

	//Intro
	if(gameState == 0){
		RECT title = {0, 100, mClientWidth, mClientHeight};
		mFont->DrawTextA(NULL, menu_Title.c_str(), menu_Title.size(), &title, DT_CENTER, RED);
	}

	// Game is underway
	if(gameState == 1){			
		RECT title = {0, 25, mClientWidth, 500};
		RECT p1_score_Rect = {25, 475, 100, 600};
		RECT p2_score_Rect = {675, 475, 800, 600};
		std::stringstream s;
		std::stringstream p1;
		std::stringstream p2;
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
		if(objects[PLAYER1]->get_Tagger()){ p1 << "\"It\"\n"; p2 << '\n';}
		else{ p2 << "\"It\"\n"; p1 << '\n'; }
		p1 << floor(player1_Score);
		p2 << floor(player2_Score);
		std::string ws = s.str();
		std::string wp1 = p1.str();
		std::string wp2 = p2.str();	
		mFont->DrawTextA(NULL, ws.c_str(), -1, &title, DT_CENTER, BLACK);
		mFont->DrawTextA(NULL, wp1.c_str(), -1, &p1_score_Rect, DT_CENTER, WHITE);
		mFont->DrawTextA(NULL, wp2.c_str(), -1, &p2_score_Rect, DT_CENTER, BLACK);
	}

	last_frame_Time = mTimer.getGameTime();
	mSwapChain->Present(0, 0);
}

void Tag::reset()
{
	gameState = 0;
	input->clearAll();
	std::string winner;
	if (player1_Score < player2_Score){ winner = "White"; P1wins++; }
	else if (player1_Score > player2_Score){ winner = "Black"; P2wins++; }
	else winner = "Nobody";

	std::string tagger="";
	if (objects[PLAYER1]->get_Tagger()) tagger = "Black";
	if (objects[PLAYER2]->get_Tagger()) tagger = "White";

	std::stringstream ss;
	ss << winner + " wins!\nWhite: " << P1wins << "  Black: " << P2wins << "\n" << tagger << " is \"It!\"\nPress any key to begin";
	menu_Title = ss.str();
	game_Seconds = 60;
	game_Minutes = (game_Seconds/60);
	player1_Score = 0;
	player2_Score = 0;
	last_frame_Time = 0;
	current_frame_Time = 0;

	objects[PLAYER1]->setPosition(Vector3(-13,0,Z));
	objects[PLAYER2]->setPosition(Vector3(13,0,Z));

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
	hr = D3DX10CreateEffectFromFile(L"SuperTagTurbo//source//color.fx", 0, 0, 
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