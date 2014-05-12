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
#include "Weapon.h"
using std::vector;
using std::string;

class BodyPart;
class Body;
class Game2App;

class Player : public Body {
public:
	Player();
	~Player(void);

	void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* device, Light* light);
	void buildBody();
	void attachApp(Game2App* _app);

	void attachBox(Box* box);

	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);

	void update(float dt);
	void draw(Matrix);
	
	void setMTech(ID3D10EffectTechnique* m);
	void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*);
	void Player::setLightingVar(Light* light);
	Vector3 getPosition() {return position;}
	void setPosition(Vector3 pos);
	Vector3 getDirection() {return direction;}
	vector<Vector3> perimeter;
	float getHeight() {return height;}

	void syncInput(Input* in) {input = in;}
	void setWeapon(Weapon* w);
	void setCollisionPoint(Vector3 p);

public:
	bool colliding;
	bool canStrafe;
	bool hasWeapon;
	bool teleporting;
	float teleportFloat;

	float teleportCooldownCounter;
	float teleportCooldown;

	Weapon* weapon;


private:
	string name;
	BodyPart* torso;
	BodyPart* head;
	BodyPart *rightArm, *leftArm, *rightForearm, *leftForearm;
	BodyPart *rightLeg, *leftLeg, *rightShin, *leftShin;
	Vector3 position;
	Vector3 direction;
	Light* spotLight;
	Vector2 mousePos, lastMousePos;

	Game2App* app;

	Box* b;

	
	Vector3 left, right, forward, back, up, down, zero;

	Vector3 collisionPoint;


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

