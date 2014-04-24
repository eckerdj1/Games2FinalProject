#pragma once
#include "Box.h"
#include "GameObject.h"
#include "constants.h"
#include "d3dUtil.h"
#include "input.h"
#include <math.h>
#include <vector>
#include <string>
#include "audio.h"
#include "BodyPart.h"
#include "CubeTex.h"
using std::vector;
using std::string;

class BodyPart;

class Weapon
{
public:
	Weapon();
	~Weapon(void);

	void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* device);
	void buildParts();

	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);

	virtual void update(float dt);
	void draw(Matrix);
	
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*);
	
	Vector3 getPosition() {return position;}
	Vector3 getDirection() {return direction;}
public:
	D3DXMATRIX mWVP;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectShaderResourceVariable* diffuseMapVar;

private:
	string name;

	vector<BodyPart*> parts;

	Vector3 position;
	Vector3 direction;

	ID3D10Device* device;
};