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

class Player : public Body {
public:
	Player();
	~Player(void);

	void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* device, Light* light);
	void buildBody();

	void attachBox(Box* box);

	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);

	void update(float dt);
	void draw(Matrix);
	
	void setMTech(ID3D10EffectTechnique* m) { mTech = m;}
	void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*);
	void Player::setLightingVar(Light* light);
	Vector3 getPosition() {return position;}
	void setPosition(Vector3 pos);
	Vector3 getDirection() {return direction;}
	vector<Vector3> perimeter;
	float getHeight() {return height;}

	void syncInput(Input* in) {input = in;}

public:
	bool colliding;
	bool canStrafe;

private:
	string name;
	BodyPart* torso;
	BodyPart* head;
	BodyPart *rightArm, *leftArm;
	BodyPart *rightLeg, *leftLeg, *rightShin, *leftShin;
	Vector3 position;
	Vector3 direction;
	Light* spotLight;
	Vector2 mousePos, lastMousePos;

	Box* b;

	
	Vector3 left, right, forward, back, up, down, zero;


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
	ID3D10EffectVariable* mfxLightVar;
	Input* input;

};

