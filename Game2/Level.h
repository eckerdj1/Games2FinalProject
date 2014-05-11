#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "GameObject.h"
#include <fstream>
#include "Box.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Pickup.h"
#include "Tower.h"
//#include "Tower"


using std::vector;

class Level {
private:
public:
	vector<Wall> walls;
	vector<Pickup> pickups;
	vector<Enemy*> enemies;
	vector<Tower*> towers;
	vector<Light*> spotLights;
	Vector3 playerLoc;
	int enlargeByC;

	void attachEnemyBox(Box*);
	void attachWallBox(Box*);

private:

	//vector<Part> parts;

	Vector3 levelDimensions;
	Vector3 exitLoc;
	Player* player;
	ID3D10Device* md3dDevice;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectShaderResourceVariable* diffuseMapVar;
	ID3D10EffectShaderResourceVariable* specMapVar;
	ID3D10EffectScalarVariable* useTexture;
	
	ID3D10ShaderResourceView* mWallTex;
	ID3D10ShaderResourceView* mWallSpec;
	
	ID3D10ShaderResourceView* mPickUpTex;
	ID3D10ShaderResourceView* mPickUpSpec;

	Box* enemyBox;
	Box* wallBox;
public:
	Level();
	~Level();
	Level(ID3D10Device* device); 

	void fillLevel(string s);
	void fillAltLevel(string s);
	void update(float dt);
	void draw(Matrix mVP);
	void setMTech(ID3D10EffectTechnique* tech) { mTech = tech; }
	void setEffectVariables(ID3D10EffectMatrixVariable* wvpVar, ID3D10EffectMatrixVariable* worldVar);
	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);
	void setSpecMap(ID3D10EffectShaderResourceVariable* var);
	void setTextureUseVar(ID3D10EffectScalarVariable* var) {useTexture = var;}
	void setPlayer(Player* p) { player = p; }
	Vector2 getLevelSize(){ return Vector2(levelDimensions.x, levelDimensions.z); }

	void reset();






};



#endif