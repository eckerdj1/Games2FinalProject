#include "Level.h"

Level::Level() {
	playerLoc = Vector3(0.0f, 0.0f, 0.0f);
	exitLoc = Vector3(0.0f, 0.0f, 0.0f);
	levelDimensions = Vector3(0.0f,0.0f,0.0f);
	enlargeByC = 5;
}

Level::~Level()
{
	//for (int i = 0; i < enemies.size(); i++) {
 //                   delete enemies[i];
 //   }
 //   for (int i = 0; i < towers.size(); i++) {
 //                   delete towers[i];
 //   }
 //   for (int i = 0; i < spotLights.size(); i++) {
 //                   delete spotLights[i];
 //   }
    //enemies.clear();s
    //towers.clear();
    //spotLights.clear();
    //player = NULL;
    //md3dDevice = NULL;
}
//Level::~Level() {
//	for (int i = 0; i < enemies.size(); i++) {
//		delete enemies[i];
//	}
//	for (int i = 0; i < towers.size(); i++) {
//		delete towers[i];
//	}
//	for (int i = 0; i < spotLights.size(); i++) {
//		delete spotLights[i];
//	}
//	enemies.resize(0);
//	towers.resize(0);
//	spotLights.resize(0);
//	player = NULL;
//	md3dDevice = NULL;
//}

Level::Level(ID3D10Device* device) {
	playerLoc = Vector3(0.0f, 0.0f, 0.0f);
	exitLoc = Vector3(0.0f, 0.0f, 0.0f);
	levelDimensions = Vector3(0.0f,0.0f,0.0f);
	md3dDevice = device;
	enlargeByC = 5;
	
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"oldBricks.jpg", 0, 0, &mWallTex, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"oldBricks.dds", 0, 0, &mWallSpec, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"pickUp.jpg", 0, 0, &mPickUpTex, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"pickUp.dds", 0, 0, &mPickUpSpec, 0 ));
}

void Level::setEffectVariables(ID3D10EffectMatrixVariable* wvpVar, ID3D10EffectMatrixVariable* worldVar)
{
	mfxWVPVar = wvpVar;
	mfxWorldVar = worldVar;
}

void Level::attachEnemyBox(Box* box)
{
	enemyBox = box;
}

void Level::attachWallBox(Box* box)
{
	wallBox = box;
}

void Level::fillLevel(string s) {

	std::ifstream fin;
	fin.open(s);

	//floor 
	fin >> levelDimensions.x;
	fin >> levelDimensions.z;
	levelDimensions.x*=enlargeByC;
	levelDimensions.z*=enlargeByC;
	// make the four walls to surround the floor
	Wall* wall;
	for (int i = 0; i < 4; i++) {
		wall = new Wall;
		wall->attachBox(wallBox);
		Vector3 position;
		Vector3 scale;
		switch(i) {
		case 0:
			position = Vector3(-levelDimensions.x/2, 0.0f, 0.0f);
			scale = Vector3(3.0f, 20.0f, levelDimensions.z);
			break;
		case 1:
			position = Vector3(0.0f, 0.0f, levelDimensions.z/2);
			scale = Vector3(levelDimensions.x, 20.0f, 3.0f);
			break;
		case 2:
			position = Vector3(levelDimensions.x/2, 0.0f, 0.0f);
			scale = Vector3(3.0f, 20.0f, levelDimensions.z);
			break;
		case 3:
			position = Vector3(0.0f, 0.0f, -levelDimensions.z/2);
			scale = Vector3(levelDimensions.x, 20.0f, 3.0f);
			break;
		}
		wall->init(md3dDevice, position, scale, Blue);
		walls.push_back(*wall);
		delete wall;
	}

	int count = 0;
	fin >> count;
	for (int i = 4; i < count+4; i++) {
		// walls
		float x1, x2, z1, z2, b;
		fin >> x1 >> z1 >> x2 >> z2 >> b;
		float xLength = x2 - x1;
		if (xLength < 0) {
			xLength *= -1;
		}
		float zLength = z2 - z1;
		if (zLength < 0) {
			zLength *= -1;
		}
		if (zLength < xLength && b == 0) {
			xLength*=enlargeByC;
		} else if (b == 0) {
			zLength*=enlargeByC;
		}
		if(b == 1) {
			xLength*=enlargeByC;
			zLength*=enlargeByC;
		}
		//assuming the x,z position is at the center of the box
		Vector3 position = Vector3(((x1*enlargeByC)) + (xLength/2), 0, ((z1*enlargeByC)) + (zLength/2));
		wall = new Wall;
		wall->attachBox(wallBox);
		wall->init(md3dDevice, position, Vector3(xLength, 20.0f, zLength), Blue);
		walls.push_back(*wall);
		delete wall;
	}
	//	Enemy Count
	fin >> count;
	Enemy* enemy;
	for (int i = 0; i < count; i++) {
		std::string type;
		int segments;
		fin >> type;
		float posX;
		float posZ;
		//enemy stuff
		if (type == "path") {
			fin >> segments;
			fin >> posX;
			fin >> posZ;
			enemy = new Enemy;
			Light* sL = new Light;
			sL->ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			sL->diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			sL->specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			sL->att.x    = 1.0f;
			sL->att.y    = 0.00099f;
			sL->att.z    = 0.0035f;
			sL->spotPow  = 2.0f;
			sL->range    = 300.0f;
			spotLights.push_back(sL);
			//enemy->setDiffuseMap(diffuseMapVar);
			enemy->setAImode(PATH);
			enemy->attachBox(enemyBox);
			//enemy->init("Enemy", Vector3(posX*enlargeByC, 0, posZ*enlargeByC), 15, 17, 6, 3.3f, md3dDevice, &spotLights[spotLights.size()-1]);
			enemy->init("Enemy", Vector3(0, 0, 0), 15, 17, 6, 3.3f, md3dDevice, sL);
			enemy->setPosition(Vector3(posX*enlargeByC, 0, posZ*enlargeByC));
			enemy->addPathPoint(Vector3(posX*enlargeByC, 0, posZ*enlargeByC));
			for (int j = 1; j < segments; j++) {
				fin >> posX;
				fin >> posZ;
				enemy->addPathPoint(Vector3(posX*enlargeByC, 0, posZ*enlargeByC));
			}
		} else if (type == "random") {
			enemy = new Enemy;
			Light* sL = new Light;
			sL->ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			sL->diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			sL->specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			sL->att.x    = 1.0f;
			sL->att.y    = 0.00099f;
			sL->att.z    = 0.0035f;
			sL->spotPow  = 2.0f;
			sL->range    = 300.0f;
			spotLights.push_back(sL);
			enemy->setAImode(RANDOM);
			enemy->attachBox(enemyBox);
			enemy->init("Enemy", Vector3(0, 0, 0), 15, 17, 6, 3.3f, md3dDevice, sL);
			float boundX1, boundX2;
			float boundZ1, boundZ2;
			fin >> boundX1; 
			fin >> boundZ1;
			fin >> boundX2;
			fin >> boundZ2;
			enemy->setPosition(Vector3(boundX1*enlargeByC, 0, boundZ1*enlargeByC));
			enemy->setBounds(Vector2(boundX1, boundX2), Vector2(boundZ1, boundZ2));
		}
		enemies.push_back(enemy);
		int temp = 0;
		temp++;
	}	
	fin >> count;
	Tower* tower;
	for (int i = 0; i < count; i++) {
		//do tower stuff
		float posX;
		float posZ;
		fin >> posX;
		fin >> posZ;
		tower = new Tower;
		Light* sL = new Light;
		sL->ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		sL->diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		sL->specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		sL->att.x    = 1.0f;
		sL->att.y    = 0.0f;
		sL->att.z    = 0.0f;
		sL->spotPow  = 64.0f;
		sL->range    = 300.0f;
		spotLights.push_back(sL);
		tower->init("Tower", Vector3(0, 0, 0), 0, 17, 6, 3.3f, md3dDevice, sL);
		tower->setDirTheta(i*90.0f);
		tower->setPosition(Vector3(posX*enlargeByC, 0, posZ*enlargeByC));
		towers.push_back(tower);
	}
	fin >> count;
	Pickup* pickup;
	for (int i = 0; i < count; i++) {
		float posX;
		float posZ;
		fin >> posX;
		fin >> posZ;
		pickup = new Pickup;
		pickup->init(md3dDevice, Vector3(posX*enlargeByC, 0.0f, posZ*enlargeByC), Vector3(2.0*enlargeByC, 2.0*enlargeByC, 2.0*enlargeByC), D3DXCOLOR(.4f, .4f, .4f, 1.0f));
		pickups.push_back(*pickup);
	}
	fin >> playerLoc.x;
	fin >> playerLoc.z;
	player->setPosition(Vector3(playerLoc.x*enlargeByC, 0.0f, playerLoc.z*enlargeByC));
	//playerLoc = player->getPosition();
	fin >> exitLoc.x;
	fin >> exitLoc.z;
	//build a landing area for the exit destination
	wall = new Wall;
	wall->attachBox(wallBox);
	wall->init(md3dDevice, Vector3(exitLoc.x*enlargeByC, 0.0f, exitLoc.z*enlargeByC), Vector3(4.0f*enlargeByC, 1.0f, 4.0f*enlargeByC), Green);
	walls.push_back(*wall);
	delete wall;
	

	fin.close();


}


void Level::update(float dt) {
	
	for(int i = 0; i < walls.size(); i++) {
		walls[i].update(dt);
	}
	for (int i = 0; i < pickups.size(); i++) {
		pickups[i].update(dt);
	}
	for (int i = 0; i < towers.size(); i++) {
		towers[i]->update(dt);
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->update(dt);
	}
}

void Level::draw(Matrix mVP) {
	diffuseMapVar->SetResource(mWallTex);
	specMapVar->SetResource(mWallSpec);
	useTexture->SetInt(1);
	for (int i = 0; i < walls.size(); i++) {
		walls[i].setMTech(mTech);
		mfxWVPVar->SetMatrix(walls[i].getWorldMatrix() * mVP);
		mfxWorldVar->SetMatrix(walls[i].getWorldMatrix());
		if (i == walls.size() - 1)
			useTexture->SetInt(0);			
		walls[i].draw();
	}
	useTexture->SetInt(1);
	diffuseMapVar->SetResource(mPickUpTex);
	specMapVar->SetResource(mPickUpSpec);
	for (int i = 0; i < pickups.size(); i++) {
		pickups[i].setMTech(mTech);
		mfxWVPVar->SetMatrix(pickups[i].getWorldMatrix() * mVP);
		mfxWorldVar->SetMatrix(pickups[i].getWorldMatrix());
		pickups[i].draw();
	}
	useTexture->SetInt(0);
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->setMTech(mTech);
		enemies[i]->setEffectVariables(mfxWVPVar, mfxWorldVar);
		enemies[i]->draw(mVP);
	}
	for (int i = 0; i < towers.size(); i++) {
		towers[i]->setMTech(mTech);
		towers[i]->setEffectVariables(mfxWVPVar, mfxWorldVar);
		towers[i]->draw(mVP);
	}

}

void Level::setDiffuseMap(ID3D10EffectShaderResourceVariable* var)
{
	diffuseMapVar = var;
}

void Level::setSpecMap(ID3D10EffectShaderResourceVariable* var)
{
	specMapVar = var;
}

void Level::reset()
{
	for (int i=0; i<pickups.size(); ++i)
		pickups[i].setActive();
}