#pragma once

#include "Body.h"

enum AiMode {RANDOM, PATH};

class Enemy : public Body {
public:
	Enemy();
	~Enemy();

	void init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* device, Light* light);
	void buildBody();

	void attachBox(Box*);

	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);

	void update(float dt);
	void draw(Matrix);
	
	void setMTech(ID3D10EffectTechnique* m) { mTech = m;}
	void setEffectVariables(ID3D10EffectMatrixVariable*, ID3D10EffectMatrixVariable*);

	Vector3 getPosition() {return position;}
	Vector3 getDirection() {return direction;}

	void setPosition(Vector3 pos);

	void setAImode(AiMode a) {aiMode = a;}
	void setBounds(Vector2 xLim, Vector2 zLim) {xBounds = xLim; zBounds = zLim;}
	void addPathPoint(Vector3 p) {aiPath.push_back(p);}
	float getRange() {return range;}

private:
	string name;
	BodyPart* torso;
	BodyPart* head;
	BodyPart *rightArm, *leftArm;
	BodyPart *rightLeg, *leftLeg, *rightShin, *leftShin;
	Light* spotLight;

	Box* b;
	
	Vector3 position;
	Vector3 direction;
	Vector3 lastLoc;
	int c;

	float height;
	float width;
	float depth;

	float range;

	float dirTheta;
	float turnSpeed;

	float gameTime;
	float elapsed;
	
	float speed;
	float normalSpeed;
	float sprintBoost;

	float limbSpeed;
	ID3D10Device* device;


	AiMode aiMode;
	Vector2 xBounds;
	Vector2 zBounds;

	vector<Vector3> aiPath;
	int pathIndex;
	int pathDir;
};