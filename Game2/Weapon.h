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

class Weapon : public Body
{
public:
	Weapon();
	~Weapon(void);

	void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* device);
	void buildBody();
	void addPart(string name, string addTo, Vector3 pos, Vector3 size);

	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);

	virtual void update(float dt);
	void draw(Matrix);
	
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*);
	
	Vector3 getPosition() {return position;}
	Vector3 getDirection() {return direction;}

	void setPosition(Vector3 pos) {position = pos;}
	void setDirection(Vector3 dir) {direction = dir;}
public:

	void attachBox(Box* b);
	void setRotX(string partName, float rot);
	void setRotY(string partName, float rot);
	void setRotZ(string partName, float rot);

	string getName() {return name;}

private:
	string name;

	vector<BodyPart*> parts;

	float speed;

	Vector3 position;
	Vector3 direction;
	Vector3 size;

	Box* box;
	ID3D10Device* device;

private:
	void addToPart(BodyPart* part, string root);
};