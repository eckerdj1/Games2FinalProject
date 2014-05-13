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
#include "Body.h"
using std::vector;
using std::string;

class BodyPart;
class Body;

class Tower : public Body{
public:
	Tower();
	~Tower(void);

	void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* d, Light* light);
	void buildBody();

	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);

	void update(float dt);
	void draw(Matrix);
	
	void setMTech(ID3D10EffectTechnique* m) {mTech = m;}
	void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*);

	Vector3 getPosition() {return position;}
	void setPosition(Vector3 pos);
	Vector3 getDirection() {return direction;}
	void setDirTheta(float dTh) { dirTheta = dTh; }
	void addAiRot(Vector2 rotData);
	float getRange() {return range;}
	
	string name;
	BodyPart* base;
	BodyPart* column;
	BodyPart* eye;

	Light* spotLight;

	void attachBox(Box* b) {box = b;}

private:
	float range;

	Vector3 position;
	Vector3 direction;

	vector<Vector2> aiRot; //Rotation amount, duration.  Duration -1.0f if no wait period
	int rotationIndex;

	float height;
	float width;
	float depth;

	float dirTheta;
	float turnSpeed;

	float gameTime;
	float elapsed;
	
	float speed;
	float normalSpeed;
	float sprintBoost;

	float limbSpeed;
	ID3D10Device* device;

	Box* box;

};