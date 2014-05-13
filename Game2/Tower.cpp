#include "Tower.h"

Tower::Tower()
{
	speed = 0;
	normalSpeed = 0;
	sprintBoost = 0;
	direction = Vector3(0,0,1);
	position = Vector3(0,0,0);
	dirTheta = 0;
	turnSpeed = 5;
	gameTime = 0;
	rotationIndex = 0;
	range = 100;
}

Tower::~Tower()
{
	delete base;
	delete column;
	delete eye;
}

void Tower::init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* d, Light* light)
{
	device = d;

	name = n;
	position = pos;
	speed = spd;
	normalSpeed = speed;
	sprintBoost = speed * 3.3f;
	limbSpeed = speed * 0.5f;
	this->height = height;
	this->width = width;
	this->depth = depth;
	buildBody();

	spotLight = light;

}

void Tower::setEffectVariables(ID3D10EffectMatrixVariable* wvpVar, ID3D10EffectMatrixVariable* worldVar)
{
	mfxWVPVar = wvpVar;
	mfxWorldVar = worldVar;
	
}

void Tower::buildBody() {
	base = new BodyPart();
	column = new BodyPart();
	eye = new BodyPart();

	Vector3 bPos = position;
	bPos = Vector3(0,0,0);
	base->init("base", box, bPos, direction, Vector3(width*2, height/15, depth*2), speed);
	base->setBody(this);

	Vector3 cPos = position;
	//cPos.y += height/2;
	column->init("column", box, cPos, direction, Vector3(width*.75, height, depth*.75), speed);
	column->setBody(this);

	Vector3 ePos = position;
	ePos.y += height;
	eye->init("eye", box, ePos, direction, Vector3(width*.80, width*.80, depth*.80), speed);
	eye->setBody(this);

	base->addChild(column);
	base->addChild(eye);

}

void Tower::update(float dt) {
	if(aiRot.size() == 0) {
		aiRot.push_back(Vector2(1.0f,-1.0f));
	}
	
	gameTime += dt;
	elapsed += dt;

	if(aiRot[rotationIndex].y == -1.0f) {
		elapsed = 0;
	}
	else if(aiRot[rotationIndex].y < elapsed) {
		rotationIndex++;

		if(rotationIndex == aiRot.size())
			rotationIndex = 0;

		elapsed = 0;
	}

	dirTheta += aiRot[rotationIndex].x * dt;

	if(dirTheta > 360.0f)
		dirTheta -= 360.0f;


	direction.x = sinf(dirTheta);
	direction.z = cosf(dirTheta);
	eye->setDirection(direction);
	base->setPosition(Vector3(position.x, position.y + height/20 * .5f, position.z));

	eye->update(dt);
	column->update(dt);
	base->update(dt);

	spotLight->pos = position + Vector3(0.0f, 4.5f+eye->getPosition().y, 0.0f);
	spotLight->range = 100.0f;
	spotLight->diffuse.r += 1.0f;
	D3DXVec3Normalize(&spotLight->dir, &(eye->getDirection() + Vector3(0.0f,-.5f,0.0f)));
}

void Tower::draw(Matrix mVP)
{
	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		base->draw(mVP);
    }
}


void Tower::setDiffuseMap(ID3D10EffectShaderResourceVariable* var)
{
	diffuseMapVar = var;
}

void Tower::addAiRot(Vector2 rotData) {
	//rotData.x contains rotation increment
	//rotData.y contains rotation duration.  -1 if infinite

	aiRot.push_back(rotData);

}

void Tower::setPosition(Vector3 pos)
{
	position = pos;
	spotLight->pos = pos + Vector3(0.0f, 4.5f, 0.0f);
}