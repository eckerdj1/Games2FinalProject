//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================


#include "Game2App.h"






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//#endif


	Game2App theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

Game2App::Game2App(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0), mFXHud(0), mTechHud(0), mVertexLayoutHud(0),
  mfxWVPVarHud(0), mTheta(0.0f), mPhi(PI*0.25f)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mVP); 
}

Game2App::~Game2App()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
	ReleaseCOM(mFXHud);
	ReleaseCOM(mVertexLayoutHud);
}

void Game2App::initApp()
{
	D3DApp::initApp();
	
	gameState = TITLE;
	playState.level = 1;
	playState.completedLevel = false;
	playState.livesRemaining = 3;
	audio->playCue(MAIN_TRACK);

	srand(time(0));
	left = Vector3(1,0,0);
	right = Vector3(-1,0,0);
	forward = Vector3(0,0,-1);
	back = Vector3(0,0,1);
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	zero = Vector3(0,0,0);

	firstPerson = 1;
	topDown = 2;
	cameraMode = topDown;
	mousePos = Vector2(0.0f, 0.0f);
	lastMousePos = mousePos;
	
	//	Splash Screen init
	float sWidth, sHeight;
	sWidth = 177.0f;
	sHeight = 132.5f;
	splash.init(md3dDevice, sWidth, sHeight, White);
	splashScreenIsUp = false;

	splashPos = Vector3(0.0f, 200.0f - (sHeight / 2.0f), 110.0f);
	splashCamPos = Vector3(0.0f, 200.0f, -50.0f);
	splashTarget = Vector3(0.0f, 200.0f, 200.0f);

	//	HUD 
	teleChargeBorder.init(md3dDevice, 2.0f, 0.4f, White);
	teleCharge.init(md3dDevice, 2.0f, 0.4f, White);
	teleHudPos = Vector3(0,0,0);
	teleChargeBorderTex.Initialize(md3dDevice, L"TeleChargeBorder.png");
	teleChargeTex.Initialize(md3dDevice, L"TeleCharge.png");

	//initialize texture resources
	diffuseMap.Initialize(md3dDevice, L"goon.jpg");
	specMap.Initialize(md3dDevice, L"defaultspec.dds");
	floorTexMap.Initialize(md3dDevice, L"floor.jpg");
	floorSpecMap.Initialize(md3dDevice, L"floor.dds");
	titleSplash.Initialize(md3dDevice, L"Stealth Tactics Title Screen.png");
	controlsSplash.Initialize(md3dDevice, L"Controls.png");
	loadSplash.Initialize(md3dDevice, L"LoadScreen.png");
	howToSplash.Initialize(md3dDevice, L"How to Play.png");
	creditsSplash.Initialize(md3dDevice, L"Credits.png");
	winSplash.Initialize(md3dDevice, L"Game Win.png");
	gameOverSplash.Initialize(md3dDevice, L"Game Over Screen.png");
	splashSpecMap.Initialize(md3dDevice, L"defaultSpec.dds");

	wallDetectionIsOn = true;

	CameraDirection = forward;
	camPos = Vector3(0.0f, 100.0f, 0.0f);
	camZoom = 1.0f;
	camTheta = 0.0f;
	camPhi = 0.0f;
	camTurnSpeed = 5;
	zoomSpeed = 5;
	maxZoom = 10.0f;
	minZoom = .8f;

	spotted = false;
	spotCounter = 0.0f;

	timer = 0.0f;


	//init lights - using pointlights
	// parallel - 0
	// point - 1
	// spotlight - 2
	lightType = 1;
	int rows = 1, cols = 1;
	numberOfLights = rows * cols;
	float startRowPos = 0;
	float startColPos = 0;
	float spacing = 20;
	for (int i=0; i<rows * cols; ++i)
	{
		Light l;
		l.pos = Vector3(((i / cols) % rows) * spacing + startRowPos,
						90,
						(i % cols) * spacing + startColPos);
		//l.pos = Vector3(0,100,0);
		l.ambient = Color3(1.0f, 1.0f, 1.0f);
		l.diffuse = Color3(1.0f, 1.0f, 1.0f);
		l.specular = Color3(1.0f, 1.0f, 1.0f);
		l.att.x = 0.0f;
		l.att.y = 0.017f;
		l.att.z = 0.00085f;
		l.range = 500.0f;
		l.type = lightType;
		lights.push_back(l);
	}
	ambientLight.pos = Vector3(0,0,0);
	ambientLight.ambient = Color3(0.17f, 0.17f, 0.17f);

	splashLight.pos = Vector3(0,150,-100);
	splashLight.dir = Vector3(0, 0, 1);
	splashLight.ambient = Color3(0.0f, 0.0f, 0.0f);
	splashLight.diffuse = Color3(1.0f, 1.0f, 1.0f, 1.0f);
	splashLight.specular = Color3(1.0f, 1.0f, 1.0f, 1.0f);
	splashLight.att.x = 1.0f;
	splashLight.att.y = 0.0f;
	splashLight.att.z = 0.0f;
	splashLight.type = 0;
	splashLight.range = 1000.0f;

	spotLight.ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	spotLight.diffuse  = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	spotLight.specular = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	spotLight.att.x    = 1.0f;
	spotLight.att.y    = 0.01f;
	spotLight.att.z    = 0.0007f;
	spotLight.spotPow  = 22.0f;
	spotLight.range    = 200.0f;

	teleportLight.ambient  = D3DXCOLOR(0.0f, .80f, 1.0f, 1.0f);
	teleportLight.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	teleportLight.specular = D3DXCOLOR(0.0f, .80f, 1.0f, 1.0f);
	teleportLight.att.x    = 1.0f;
	teleportLight.att.y    = 0.00f;
	teleportLight.att.z    = 0.000f;
	teleportLight.spotPow  = 64.0f;
	teleportLight.range    = 0.0f;

	playerBox = new Box();
	enemyBox = new Box();
	wallBox = new Box();
	blackBox = new Box();

	playerBox->init(md3dDevice, 1, 1, 1, LightBlue, LightBlue);
	player.attachBox(playerBox);
	player.init("Daniel", Vector3(0, 0, 0), 20, 17, 6, 3.3f, md3dDevice, &spotLight, &teleportLight);
	player.attachApp(this);

	enemyBox->init(md3dDevice, 1, 1, 1, DarkRed, DarkRed);
	wallBox->init(md3dDevice, 1, 1, 1, White, White);
	wallBox->setDiffuseMap(mfxDiffuseMapVar);
	blackBox->init(md3dDevice, 1, 1, 1, White, White);

	//level1 = new Level(md3dDevice);
	//level1->setPlayer(&player);
	//level1->fillLevel("level1.txt");
	
	//level2 = new Level(md3dDevice);
	//level2->setPlayer(&player);
	//level2->fillLevel("level2.txt");

	//level3 = new Level(md3dDevice);
	//level3->setPlayer(&player);
	//level3->fillLevel("level3.txt");
	level = 0;
	level1 = 0;
	level2 = 0;
	level3 = 0;
	level4 = 0;
	level5 = 0;
	numberOfSpotLights = 30; 
		
	target = splashTarget;
	

	//level = level1;
	floor.init(md3dDevice, 1000, 1000);
	//playState.pickUpsRemaining = level->pickups.size();
	buildFX();
	buildHudFX();
	buildVertexLayouts();

	//numberOfSpotLights = level->spotLights.size();

	player.setDiffuseMap(mfxDiffuseMapVar);
	player.syncInput(input);

	//	Creating the Teleportation Gun
	teleportGun = new Weapon();
	teleportGun->attachBox(blackBox);
	Vector3 gunSize(2.0, 1.5, 5.0);
	teleportGun->init("TeleportGun", Vector3(0, 10, 0), 15, gunSize.x, gunSize.y, gunSize.z, md3dDevice);

	//	Add parts to the gun
	teleportGun->addPart("Body", "", Vector3(0,0,0), gunSize);
	teleportGun->addPart("FrontHandle", "Body", Vector3(0, gunSize.y * 0.5f, gunSize.z * 0.3f),
		Vector3(gunSize.x * 2.87f, gunSize.y * 0.25f, gunSize.y * 0.25f));
	teleportGun->setRotX("FrontHandle", 0.785f);
	teleportGun->addPart("Barrel1", "Body", Vector3(0, gunSize.y * 0.3f, 3.0f),
		Vector3(gunSize.x * 0.4f, gunSize.y * 0.4f, 1.0));
	teleportGun->addPart("Barrel2", "Barrel1", Vector3(0, gunSize.y * 0.3f * 0.4f, 0.585f),
		Vector3(gunSize.y * 0.2f, gunSize.y * 0.2f, 0.37f));
	teleportGun->addPart("TopHandleRight", "Body", Vector3(0.77f, 1.5f, -1.7f),
		Vector3(0.2f, 1.0f, 0.2f));
	teleportGun->addPart("TopHandleLeft", "Body", Vector3(-0.77f, 1.5f, -1.7f),
		Vector3(0.2f, 1.0f, 0.2f));
	teleportGun->addPart("TopHandleTop", "Body", Vector3(0.0f, 2.5f, -1.7f),
		Vector3(1.74f, 0.2f, 0.2f));
	teleportGun->addPart("LeftPanel", "Body", Vector3(-1.0f, 0.75f, -0.7f),
		Vector3(0.75f, 0.1f, 3.0f));
	teleportGun->setRotZ("LeftPanel", 1.57f);
	teleportGun->addPart("RightPanel", "Body", Vector3(1.0f, 0.75f, -0.7f),
		Vector3(0.75f, 0.1f, 3.0f));
	teleportGun->setRotZ("RightPanel", -1.57f);
	teleportGun->addPart("TopFrontPanel", "Body", Vector3(0.0f, 1.5f, 1.7f),
		Vector3(1.1f, 0.1f, 1.0f));
	teleportGun->addPart("TopMiddlePanel", "Body", Vector3(0.0f, 1.5f, 0.3f),
		Vector3(1.1f, 0.1f, 1.0f));
	teleportGun->addPart("TopBackPanel", "Body", Vector3(0.0f, 1.5f, -1.1f),
		Vector3(1.1f, 0.1f, 1.0f));

	player.setWeapon(teleportGun);
	
	//delete spotLight;
	//level->setDiffuseMap(mfxDiffuseMapVar);
	//level->setSpecMap(mfxSpecMapVar);
	
	mfxSpotCount->SetInt(0);
	mfxLightCount->SetInt(numberOfLights);
	mfxTexVar->SetInt(0);
}

void Game2App::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 5000.0f);
}

void Game2App::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	
	splashScreenIsUp = false;

	float gameTime = mTimer.getGameTime();
	
	lastMousePos = mousePos;
	mousePos = Vector2(input->getMouseRawX(), input->getMouseRawY());
	//target = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 lookAt(0.0f, 0.0f, 0.0f);

	//	Exit the game
	if (keyPressed(VK_ESCAPE))
	{
		exit(0);
	}
	//if (keyPressed(PauseKey))
	//{
	//	D3DApp::togglePause();
	//}

	if (gameState == TITLE) 
	{
		showSplash();
		//change camera, to point at splash quad
		splash.setPosition(splashPos);
		//splash.setRotXAngle(6.28f);
		splash.update(dt);
		
		camPos = splashCamPos;

		
		if (gameTime > 7.0f || keyPressed(AdvanceScreenKey))
			gameState = HOWTO;
	}
	else if (gameState == HOWTO) 
	{
		//change texture on splash to "how to" 	bool toLoading = false;
		showSplash();
		timer += dt;
		bool toLoading = false;
		if (timer > 15.0f || keyPressed(AdvanceScreenKey))
		{
			switch(playState.level)
			{
			case 1:
				if (level1 == 0)
					toLoading = true;
				break;
			case 2:
				if (level2 == 0)
					toLoading = true;
				break;
			case 3:
				if (level3 == 0)
					toLoading = true;
				break;
			case 4:
				if (level4 == 0)
					toLoading = true;
				break;
			case 5:
				if (level5 == 0)
					toLoading = true;
				break;
			}
			if (toLoading)
			{
				gameState = LOADING;
				splashScreenIsUp = true;
			}
			else 
				gameState = PLAY;
			timer = 0.0f;
		}
	} 
	else if (gameState == LEVELWIN) 
	{
		showSplash();
		playState.level += 1;
		playState.livesRemaining = 3;
		playState.newLevel = true;
		if (playState.level > 5) 
		{
			gameState = GAMEWIN;
		} 
		else 
		{
			bool toLoading = false;
			switch(playState.level)
			{
			case 1:
				if (level1 == 0)
					toLoading = true;
				break;
			case 2:
				if (level2 == 0)
					toLoading = true;
				break;
			case 3:
				if (level3 == 0)
					toLoading = true;
				break;
			case 4:
				if (level4 == 0)
					toLoading = true;
				break;
			case 5:
				if (level5 == 0)
					toLoading = true;
				break;
			}
			if (toLoading)
			{
				gameState = LOADING;
				splashScreenIsUp = true;
			}
			else 
				gameState = PLAY;
		}
	}
	else if (gameState == GAMEWIN) 
	{
		showSplash();
		timer += dt;
		if (timer > 5.0f || keyPressed(AdvanceScreenKey))
		{
			timer = 0.0f;
			gameState = CREDITS;
		}
	}
	else if (gameState == GAMEOVER) 
	{
		showSplash();
		// change camera to point at game over quad
		timer += dt;
		activeMessage = true;
		message = L"Game Over";
		if (timer > 5.0f)
		{
			activeMessage = false;
			timer = 0.0f;
			gameState = CREDITS;
		}
	} 
	else if (gameState == CREDITS) 
	{
		//display the Credits Splash screen
		showSplash();
		playState.level = 1;
		playState.newLevel = true;
		playState.livesRemaining = 3;
		playState.pickUpsRemaining = level->pickups.size();

		bool toLoading = false;
		switch(playState.level)
		{
		case 1:
			if (level1 == 0)
				toLoading = true;
			break;
		case 2:
			if (level2 == 0)
				toLoading = true;
			break;
		case 3:
			if (level3 == 0)
				toLoading = true;
			break;
		case 4:
			if (level4 == 0)
				toLoading = true;
			break;
		case 5:
			if (level5 == 0)
				toLoading = true;
			break;
		}
		if (toLoading)
		{
			gameState = LOADING;
			splashScreenIsUp = true;
		}
		else 
			gameState = PLAY;
	} 
	else if (gameState == LOADING)
	{
		showSplash();
		if (playState.level == 1)
		{
			if (!level1)
			{
				level1 = new Level(md3dDevice);
				level1->setPlayer(&player);
				level1->attachEnemyBox(enemyBox);
				level1->attachWallBox(wallBox);
				level1->fillLevel("level1.txt");
			}

			level = level1;
		}
		if (playState.level == 2)
		{
			if (!level2)
			{
				level2 = new Level(md3dDevice);
				level2->setPlayer(&player);
				level2->attachEnemyBox(enemyBox);
				level2->attachWallBox(wallBox);
				level2->fillLevel("level2.txt");
			}

			level = level2;
		} 
		if (playState.level == 3) 
		{
			if (!level3)
			{
				level3 = new Level(md3dDevice);
				level3->setPlayer(&player);
				level3->attachEnemyBox(enemyBox);
				level3->attachWallBox(wallBox);
				level3->fillLevel("level3.txt");
			}

			level = level3;
		}
		if (playState.level == 4) 
		{
			if (!level4)
			{
				level4 = new Level(md3dDevice);
				level4->setPlayer(&player);
				level4->attachEnemyBox(enemyBox);
				level4->attachWallBox(wallBox);
				level4->fillLevel("levelBackstab1.txt");
			}

			level = level4;
		}
		if (playState.level == 5) 
		{
			if (!level5)
			{
				level5 = new Level(md3dDevice);
				level5->setPlayer(&player);
				level5->attachEnemyBox(enemyBox);
				level5->attachWallBox(wallBox);
				level5->fillLevel("levelTeleport1.txt");
			}

			level = level5;
		}
		level->reset();

		level->setDiffuseMap(mfxDiffuseMapVar);
		level->setSpecMap(mfxSpecMapVar);
		numberOfSpotLights = level->spotLights.size();
		mfxSpotCount->SetInt(numberOfSpotLights);
		//playState.newLevel = false;
		playState.pickUpsRemaining = level->pickups.size();
		player.setPosition(level->playerLoc * level->enlargeByC);
		
		gameState = PLAY;
	}
	else if (gameState == PLAY) 
	{

		if (playState.newLevel)
		{
			switch(playState.level)
			{
			case 1:
				level = level1;
				break;
			case 2:
				level = level2;
				break;
			case 3:
				level = level3;
				break;
			case 4:
				level = level4;
				break;
			case 5:
				level = level5;
				break;
			}
			level->reset();
			numberOfSpotLights = level->spotLights.size();
			mfxSpotCount->SetInt(numberOfSpotLights);
			player.setPosition(level->playerLoc * level->enlargeByC);
			playState.newLevel = false;
			playState.pickUpsRemaining = level->pickups.size();
		}
		Vector3 oldPlayerPos = player.getPosition();
		vector<Vector3> oldPerimeter;
		for (int i = 0; i < player.perimeter.size(); i++) {
			oldPerimeter.push_back(player.perimeter[i]);
		}
		spotted = false;
		
		// enemy sight detection
		for (int i=0; i<level->enemies.size(); ++i)
		{
			Enemy* e = level->enemies[i];
			Vector3 toPlayer = e->getPosition() - player.getPosition();
			Vector3 norm;
			Normalize(&norm, &(-toPlayer));
			float r = max(D3DXVec3Dot(&(norm), &e->getDirection()), 0);
			float l = Length(&toPlayer);
			if (r > 0.5f && l < e->getRange())
			{
				spotted = true;
				Ray ray = Ray(e->getPosition(), -toPlayer);
				if (wallDetectionIsOn)
				{
					for (int i=0; i<level->walls.size(); ++i)
					{
						Wall w = level->walls[i];
						Box2 b = Box2(w.getPosition() - Vector3(w.getRadii().x, 0.0f, w.getRadii().z),
							w.getPosition() + Vector3(w.getRadii().x, w.getSize().y, w.getRadii().z));
				
						if (b.intersect(ray, 0.0f, 1.0f))
						{
							spotted = false;
						}
					}
				}
			}
		}
		for (int i=0; i<level->towers.size(); ++i)
		{
			Tower* t = level->towers[i];
			Vector3 toPlayer = t->getPosition() - player.getPosition();
			Vector3 norm;
			Normalize(&norm, &(-toPlayer));
			float r = max(D3DXVec3Dot(&(norm), &t->getDirection()), 0);
			float l = Length(&toPlayer);
			if (r > 0.85f && l < t->getRange())
			{
				spotted = true;
				Ray ray = Ray(t->getPosition(), -toPlayer);
				if (wallDetectionIsOn)
				{
					for (int i=0; i<level->walls.size(); ++i)
					{
						Wall w = level->walls[i];
						Box2 b = Box2(w.getPosition() - Vector3(w.getRadii().x, 0.0f, w.getRadii().z),
							w.getPosition() + Vector3(w.getRadii().x, w.getSize().y, w.getRadii().z));
				
						if (b.intersect(ray, 0.0f, 1.0f))
						{
							spotted = false;
						}
					}
				}
			}
		}
		if(spotted && spotCounter == 0) {
			spotCounter += 1.37f;
			audio->playCue(ALARM);
			//playState.livesRemaining--;
		}
		spotCounter -= dt;
		if (spotCounter < 0.0f)
			spotCounter = 0.0f;

		//collision stuff
		for (int i = 0; i < level->pickups.size(); i++) {
			if (level->pickups[i].contains(player.getPosition())) {
				if (level->pickups[i].isActive()) {
					audio->playCue(PICKUP);
					playState.pickUpsRemaining --;
				}
				level->pickups[i].setInActive();
			}
		}
		player.setCollisionPoint(Vector3(0,0,0));
		for (int i = 0; i < level->walls.size(); i++) {
			for (int j = 0; j < player.perimeter.size(); j++) {
				if (level->walls[i].contains(player.perimeter[j])) {
					if (i != level->walls.size() -1)
					{
						player.setCollisionPoint(player.perimeter[j]);
						player.colliding = true;
						//player.setPosition(oldPlayerPos);
						/*for (int k = 0; k < player.perimeter.size(); k++) {
							player.perimeter[k] = oldPerimeter[k];
						}*/
					}
					else
						if (playState.pickUpsRemaining == 0)
						{
							playState.completedLevel = true;
							gameState = LEVELWIN;
						}
					//level->walls[i].setInActive();
				}
			}
		}

		player.update(dt);
		floor.update(dt);
		level->update(dt);

		if (playState.livesRemaining == 0)
		{
			gameState = GAMEOVER;
		}

		CameraDirection.x = sinf(camTheta);
		CameraDirection.z = cosf(camTheta);

		

		// Build the view matrix.
		if (cameraMode == topDown)
		{
			player.canStrafe = false;
			camPos = Vector3(0.0f, 150.0f / camZoom, 0.0f);
			camPos += player.getPosition();
			camPos -= CameraDirection * 150 / camZoom;
			target = player.getPosition();
			//camPos = Vector3(0.0f, 100.0f, 0.0f);
		}
		else if (cameraMode == firstPerson)
		{
			player.canStrafe = true;
			camPos = Vector3(0.0f, player.getHeight(), 0.0f);
			camPos += player.getPosition();
			target = player.getPosition() + Vector3(0.0f, player.getHeight(), 0.0f);
			target += player.getDirection() * 20;
		}
		if (keyPressed(PlayerJumpKey))
		{
			//target = Vector3(0.0f, 150.0f, 100.0f);
		}

		if (keyPressed(CameraLeftKey))
		{
			camTheta -= camTurnSpeed * dt;
			if (camTheta > 180 || camTheta < -180)
				camTheta = -camTheta;
		}
		if (keyPressed(CameraRightKey))
		{
			camTheta += camTurnSpeed * dt;
			if (camTheta > 180 || camTheta < -180)
				camTheta = -camTheta;
		}
		if (keyPressed(CameraZoomInKey))
		{
			camZoom += zoomSpeed * dt;
			if (camZoom > maxZoom)
				camZoom = maxZoom;
		}
		if (keyPressed(CameraZoomOutKey))
		{
			camZoom -= zoomSpeed * dt;
			if (camZoom < minZoom)
				camZoom = minZoom;
		}
		if (input->wasKeyPressed(FirstPersonKey))
		{
			cameraMode = firstPerson;
		}
		if (input->wasKeyPressed(TopDownKey))
		{
			cameraMode = topDown;
		}
		// mouse camera control
		if (cameraMode == firstPerson)
		{
			camPhi += mousePos.y * dt;
		}
		if (input->wasKeyPressed(VK_RETURN))
		{
			if (wallDetectionIsOn)
				wallDetectionIsOn = false;
			else
				wallDetectionIsOn = true;
		}
		/*if (keyPressed(VK_RBUTTON))
		{
			camTheta += float(mousePos.x) * dt;
			camZoom -= float(mousePos.y) * dt;
			if (camTheta > 180 || camTheta < -180)
				camTheta = -camTheta;
			if (camZoom < minZoom)
				camZoom = minZoom;
			if (camZoom > maxZoom)
				camZoom = maxZoom;
		}*/
	
		
		//	Position the HUD
		targetDir = target - camPos;
		Normalize(&targetDir, &targetDir);
		teleCharge.setRotYAngle(camTheta);
		teleChargeBorder.setRotYAngle(camTheta);
		Cross(&targetRight, &Vector3(0,1,0), &targetDir);
		Normalize(&targetRight, &targetRight);
		Cross(&targetUp, &targetDir, &targetRight);
		Normalize(&targetUp, &targetUp);
		float angle = 0.78f;
		float hudOffset = 3.7f;
		teleHudPos = camPos + targetDir * 10.0f + targetRight * hudOffset + targetUp * hudOffset;
		teleCharge.setPosition(teleHudPos);
		teleHudPos = camPos + targetDir * 9.99f + targetRight * hudOffset + targetUp * hudOffset;
		teleChargeBorder.setPosition(teleHudPos);
		teleCharge.setRotXAngle(angle);
		teleChargeBorder.setRotXAngle(angle);
		teleCharge.update(dt);
		teleChargeBorder.update(dt);
		
	}

	if (gameState == LOADING)
	{
		splashScreenIsUp = true;
		camPos = splashCamPos;
		target = splashTarget;
	}
	
	//pos -= Vector3(player.getDirection().x, -0.6f, player.getDirection().z) * 80.0f;
	
	
	
	
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &camPos, &target, &up);
	input->clearAll();
}

void Game2App::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);

	// set lighting shader variables
	mfxEyePosVar->SetRawValue(&camPos, 0, sizeof(Vector3));
	
	for (int i=0; i<numberOfLights; ++i)
	{
		mfxLightVar[i]->SetRawValue(&lights[i], 0, sizeof(Light));
	}
	mfxAmbientVar->SetRawValue(&ambientLight, 0, sizeof(Light));
	mfxSplashLightVar->SetInt(0);
	if (splashScreenIsUp)
	{
		mfxSplashLight->SetRawValue(&splashLight, 0, sizeof(Light));
		mfxAmbientVar->SetRawValue(&splashLight, 0, sizeof(Light));
		mfxSplashLightVar->SetInt(1);
		showSplash();
	}
	mfxLightType->SetInt(lightType);

	// light for Player
	mfxSpotVar->SetRawValue(&spotLight, 0, sizeof(Light));
	mfxTeleVar->SetRawValue(&teleportLight, 0, sizeof(Light));
	// light for Enemies
	/*for (int i = 0; i < numberOfSpotLights; i++) {
		mfxSpotVars.resize(level->spotLights.size());
	}*/
	if (level)
	{
		for (int i = 0; i < level->spotLights.size(); i++) {
			mfxSpotVars[i]->SetRawValue(level->spotLights[i], 0, sizeof(Light));
		}
	}

	// set some variables for the shader
	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);


	// Set the resoure variables
	
	mfxDiffuseMapVar->SetResource(diffuseMap.GetTexture());
	mfxSpecMapVar->SetResource(specMap.GetTexture());

	// Set the texture matrix
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);

	
	mVP = mView * mProj;
	if (gameState == PLAY)
	{// Drawing the Player
		mfxTexVar->SetInt(0);
		player.setMTech(mTech);
		player.setEffectVariables(mfxWVPVar, mfxWorldVar);
		player.draw(mVP);
	
		//D3DApp::setCursorShow(false);
	//Drawing the level
		if (level)
		{
			level->setTextureUseVar(mfxTexVar);
			level->setDiffuseMap(mfxDiffuseMapVar);
			mfxTexVar->SetInt(0);
			mVP = mView * mProj;
			level->setMTech(mTech);
			level->setEffectVariables(mfxWVPVar, mfxWorldVar);
			level->draw(mVP);
		}

		mfxTexVar->SetInt(1);
		mfxDiffuseMapVar->SetResource(floorTexMap.GetTexture());
		mfxSpecMapVar->SetResource(floorSpecMap.GetTexture());
		floor.setMTech(mTech);
		mfxWVPVar->SetMatrix(floor.getWorldMatrix() * mVP);
		mfxWorldVar->SetMatrix(floor.getWorldMatrix());
		floor.draw();
		mfxTexVar->SetInt(0);

	}
	//Draw splash screen
	if (gameState == TITLE)
	{
		mfxDiffuseMapVar->SetResource(titleSplash.GetTexture());
	}
	if (gameState == LOADING)
	{
		mfxDiffuseMapVar->SetResource(loadSplash.GetTexture());
	}
	if (gameState == GAMEWIN)
	{
		mfxDiffuseMapVar->SetResource(winSplash.GetTexture());
	}
	if (gameState == HOWTO)
	{
		mfxDiffuseMapVar->SetResource(howToSplash.GetTexture());
	}
		//Identity(&mVP);
	if (gameState != PLAY)
	{
		mfxTexVar->SetInt(1);
		mfxSpecMapVar->SetResource(splashSpecMap.GetTexture());
		Scale(&texMtx, 1.0f, 1.0f, 1.0f);
		mfxTexMtxVar->SetMatrix((float*)&texMtx);
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			mfxWVPVar->SetMatrix(splash.getWorld() * mVP);
			mfxWorldVar->SetMatrix(splash.getWorld());
			mTech->GetPassByIndex( p )->Apply(0);
			splash.draw();
		}
	}
	else
	{
		
		D3D10_TECHNIQUE_DESC techDescHud;
		mTechHud->GetDesc(&techDescHud);
		//	Charge bar
		mfxDiffuseMapVarHud->SetResource(teleChargeTex.GetTexture());
		teleCharge.setScaleX((player.teleportChargeCounter / player.teleportChargeTime) * 2.0f);
		mfxTexMtxVarHud->SetMatrix((float*)&texMtx);
		for(UINT p = 0; p < techDescHud.Passes; ++p)
		{
			mfxWVPVarHud->SetMatrix(teleCharge.getWorld() * mVP);
			mfxWorldVarHud->SetMatrix(teleCharge.getWorld());
			mTechHud->GetPassByIndex( p )->Apply(0);
			teleCharge.draw();
		}
		//	Charge Bar Border
		mfxDiffuseMapVarHud->SetResource(teleChargeBorderTex.GetTexture());
		mfxTexMtxVarHud->SetMatrix((float*)&texMtx);
		for(UINT p = 0; p < techDescHud.Passes; ++p)
		{
			mfxWVPVarHud->SetMatrix(teleChargeBorder.getWorld() * mVP);
			mfxWorldVarHud->SetMatrix(teleChargeBorder.getWorld());
			mTechHud->GetPassByIndex( p )->Apply(0);
			teleChargeBorder.draw();
		}
	}
	


	/////Text Drawing Section
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {100, 5, 0, 0};
	RECT R1 = {0, 0, 800, 600};
	RECT R2 = {0, 540, 800, 600};

	std::wostringstream outs;  
	
	outs.precision(6);
	outs << "Lives Remaining: " << playState.livesRemaining;
	outs << "   MousePos X: "  << D3DApp::MousePos.x << "  Y: " << D3DApp::MousePos.y << "\n";
	outs << "Pick ups Left: " << playState.pickUpsRemaining;
	outs << "  Teleport Charge Counter: " << player.teleportChargeCounter << "\n";
	outs << "TargetRight: " << targetRight.x << "   " << targetRight.y << "   " << targetRight.z << "\n";
	outs << "TargetUp: " << targetUp.x << "   " << targetUp.y << "   " << targetUp.z << "\n";
	outs << "QuadRotX: " << teleCharge.rotX << "\n";
	//outs << "Game Time: " << mTimer.getGameTime() << "\n";
	//outs << "CameraPos: " << camPos.x << ", " << camPos.y << ", " << camPos.z << "\n";
	//outs << "Target: " << target.x << ", " << target.y << ", " << target.z << "\n";
	if (spotted)
		outs << "You are spotted!\n";
	
	string Hud = score.getString();
	

	/*outs << score.getString() << L"\n";
	outs << L"Blobs Available: " << ammo << L"\n";
	outs << L"Gallons Left: " << lives;*/
	std::wstring text = outs.str();
	if (gameState == PLAY)
		mFont->DrawText(0, text.c_str(), -1, &R1, DT_NOCLIP, White);
	//timesNew.draw(Hud, Vector2(5, 5));
	/*if (gameOver)
	{
		mFont->DrawText(0, L"Game Over!", -1, &R1, DT_CENTER | DT_VCENTER, Black);
	}
	float gameTime = mTimer.getGameTime();
	if (gameTime < 3.0f)
	{
		mFont->DrawText(0, L"Move your Box LEFT and RIGHT with A & D to avoid hitting the obstacles", -1, &R2, DT_CENTER | DT_VCENTER, Black);
	}
	else if (gameTime < 6.0f)
	{
		mFont->DrawText(0, L"Change the color of your Box by pressing the J and L keys.", -1, &R2, DT_CENTER | DT_VCENTER, Black);
	}
	else if (gameTime < 9.0f)
	{
		mFont->DrawText(0, L"The closer the color of your cube is to the floor, the higher the score multiplier!", -1, &R2, DT_CENTER | DT_VCENTER, Black);
	}*/
	if (activeMessage)
	{
		mFont->DrawText(0, message.c_str(), -1, &R2, DT_CENTER | DT_VCENTER, White);
	}
	

	mSwapChain->Present(0, 0);
}

void Game2App::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"lighting.fx", 0, 0, 
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

	mTech = mFX->GetTechniqueByName("LightTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar  = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVar = mFX->GetVariableByName("gEyePosW");
	for (int i=0; i<numberOfLights; ++i)
	{
		ID3D10EffectVariable* var = mFX->GetVariableByName("pointLights")->GetElement(i);
		mfxLightVar.push_back(var);
	}
	mfxAmbientVar = mFX->GetVariableByName("ambientLight");
	mfxSpotVar = mFX->GetVariableByName("spotLight");
	mfxTeleVar = mFX->GetVariableByName("teleportLight");
	for (int i = 0; i < numberOfSpotLights; i++)
	{
		ID3D10EffectVariable* vars = mFX->GetVariableByName("spotLights")->GetElement(i);
		mfxSpotVars.push_back(vars);
	}
	mfxLightType = mFX->GetVariableByName("gLightType")->AsScalar();
	mfxLightCount = mFX->GetVariableByName("numberOfLights")->AsScalar();
	mfxSpotCount = mFX->GetVariableByName("numberOfSpotLights")->AsScalar();
	mfxTexVar = mFX->GetVariableByName("useTex")->AsScalar();

	mfxSplashLight = mFX->GetVariableByName("splashLight");
	mfxSplashLightVar = mFX->GetVariableByName("useSplashLight")->AsScalar();

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
	//mfxFLIPVar = mFX->GetVariableByName("flip");

}

void Game2App::buildHudFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"hudLighting.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFXHud, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTechHud = mFXHud->GetTechniqueByName("HudLightTech");
	
	mfxWVPVarHud = mFXHud->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVarHud  = mFXHud->GetVariableByName("gWorld")->AsMatrix();


	mfxDiffuseMapVarHud = mFXHud->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxTexMtxVarHud     = mFXHud->GetVariableByName("gTexMtx")->AsMatrix();

}

void Game2App::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"DIFFUSE",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 56, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 5, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));	
	// Create the input layout for Hud
    D3D10_PASS_DESC PassDescHud;
    mTechHud->GetPassByIndex(0)->GetDesc(&PassDescHud);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 5, PassDescHud.pIAInputSignature,
		PassDescHud.IAInputSignatureSize, &mVertexLayoutHud));
}






// Optimized method
bool Box2::intersect(const Ray &r, float t0, float t1){
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = (bounds[r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tmax = (bounds[1-r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tymin = (bounds[r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	tymax = (bounds[1-r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	if ( (tmin > tymax) || (tymin > tmax) )
	return false;
	if (tymin > tmin)
	tmin = tymin;
	if (tymax < tmax)
	tmax = tymax;
	tzmin = (bounds[r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	tzmax = (bounds[1-r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ( (tmin < t1) && (tmax > t0) );
}

void Game2App::showSplash()
{
	splashScreenIsUp = true;
	camPos = splashCamPos;
	target = splashTarget;
}