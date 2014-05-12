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
#include "Light.h"
using std::string;

class Body {
public:
	Body(void);
	~Body(void);

	virtual void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* device, Light* light)
	{

	}
	virtual void buildBody() = 0;

	virtual void setDiffuseMap(ID3D10EffectShaderResourceVariable* var) = 0;

	virtual void update(float dt) = 0;
	virtual void draw(Matrix) = 0;
	
	virtual void setMTech(ID3D10EffectTechnique* m) = 0;
	virtual void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*) = 0;

	virtual Vector3 getPosition() = 0;
	virtual Vector3 getDirection() = 0;

	D3DXMATRIX mWVP;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectShaderResourceVariable* diffuseMapVar;

private:

	

};