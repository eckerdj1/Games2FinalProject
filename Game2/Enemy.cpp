#include "Enemy.h"
#include <random>

Enemy::Enemy()
{
	speed = 0;
	normalSpeed = 0;
	sprintBoost = 0;
	direction = Vector3(0,0,1);
	position = Vector3(0,0,0);
	dirTheta = 0;
	turnSpeed = 5;
	gameTime = 0;
	range = 40;
	elapsed = 0;
	active = true;

	aiMode = RANDOM;
	xBounds = Vector2(-10.0f, 10.0f);
	zBounds = Vector2(-10.0f, 10.0f);
	pathIndex = 0;
	pathDir = 1;
}

Enemy::~Enemy()
{
	delete torso;
	delete head;
	delete rightArm;
	delete leftArm;
	delete rightLeg;
	delete leftLeg;

}

void Enemy::init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* d, Light* light)
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

	spotLight = light;

	buildBody();
}

void Enemy::attachBox(Box* box)
{
	b = box;
}

void Enemy::setEffectVariables(ID3D10EffectMatrixVariable* wvpVar, ID3D10EffectMatrixVariable* worldVar)
{
	mfxWVPVar = wvpVar;
	mfxWorldVar = worldVar;
}

void Enemy::buildBody()
{

	torso = new BodyPart();
	head = new BodyPart();
	rightArm = new BodyPart();
	leftArm = new BodyPart();
	rightLeg = new BodyPart();
	leftLeg = new BodyPart();
	rightShin = new BodyPart();
	leftShin = new BodyPart();

	// torso
	Vector3 tPos = position;
	tPos.y += height * 0.5f;
	tPos = Vector3(0, 0, 0);
	torso->init("torso", b, tPos, direction, Vector3(width, height * 0.36f, depth), speed);
	torso->setBody(this);

	// head
	Vector3 hPos = position;
	hPos.y += height * 0.38f;
	head->init("head", b, hPos, direction, Vector3(width * 0.45f, height * 0.12f, depth * 0.9f), speed);
	head->setBody(this);
	head->setRoot(torso);

	// right arm
	Vector3 raPos = position;
	Vector3 torsoSize = torso->getSize();
	raPos += Vector3(width * 0.65f, torsoSize.y * 0.8f, 0);
	rightArm->init("rightArm", b, raPos, direction, Vector3(width * 0.2f, height * 0.37f, depth * 0.5f), speed);
	rightArm->setBody(this);
	rightArm->setRoot(torso);

	// left arm
	Vector3 laPos = position;
	laPos += Vector3(width * -0.65f, torsoSize.y * 0.8f, 0);
	leftArm->init("leftArm", b, laPos, direction, Vector3(width * 0.2f, height * 0.37f, depth * 0.5f), speed);
	leftArm->setBody(this);
	leftArm->setRoot(torso);

	// right leg
	Vector3 rlPos = position;
	rlPos += Vector3(width * 0.35f, height * -0.02f, 0);
	rightLeg->init("rightLeg", b, rlPos, direction, Vector3(width * 0.25f, height * 0.21f, depth * 0.5f), speed);
	rightLeg->setBody(this);
	rightLeg->setRoot(torso);

	// left leg
	Vector3 llPos = position;
	llPos += Vector3(width * -0.35f, height * -0.02f, 0);
	leftLeg->init("leftLeg", b, llPos, direction, Vector3(width * 0.25f, height * 0.21f, depth * 0.5f), speed);
	leftLeg->setBody(this);
	leftLeg->setRoot(torso);

	//right shin
	Vector3 rsPos = position;
	rsPos += Vector3(0, height * .22f, 0);
	rightShin->init("rightShin", b, rsPos, direction, Vector3(width * 0.24f, height * 0.20f, depth * 0.5f), speed);
	rightShin->setBody(this);
	rightShin->setRoot(rightLeg);

	//left shin
	Vector3 lsPos = position;
	lsPos += Vector3(0, height * .22f, 0);
	leftShin->init("leftShin", b, lsPos, direction, Vector3(width * 0.24f, height * 0.20f, depth * 0.5f), speed);
	leftShin->setBody(this);
	leftShin->setRoot(leftLeg);

	// connect body parts
	torso->addChild(head);
	torso->addChild(rightArm);
	torso->addChild(leftArm);
	torso->addChild(rightLeg);
	torso->addChild(leftLeg);
	rightLeg->addChild(rightShin);
	leftLeg->addChild(leftShin);
}

void Enemy::turnOffLight()
{
	spotLight->range = 0;
}

void Enemy::update(float dt)
{
	//No keyboard input, just preset patterns
	if (!active)
		return;
	gameTime += dt;
	elapsed += dt;
	
	bool moving = false;
	bool sprinting = false;


	switch(aiMode) {
	case RANDOM:
		//Check if first time through. If so, select random waypoint within bounds
		if(aiPath.size() == 0) {
			c= 0;
			lastLoc = position;
			float xRand, zRand;
 			std::random_device rseed;
			std::mt19937 rng(rseed());
			std::uniform_int<int> dist1(xBounds.x,xBounds.y);
			xRand = dist1(rng)*5;
			std::uniform_int<int> dist2(zBounds.x,zBounds.y);
			zRand = dist2(rng)*5;

			//aiPath.push_back(Vector3(xRand, position.y, zRand));
			aiPath.push_back(Vector3(xBounds.y*5, position.y, zBounds.y*5));
			direction = aiPath[0] - position;
			D3DXVec3Normalize(&direction, &direction);

			elapsed = 0;
		}
		
		//Check if close to destination
		//	If so, calculate and set new destination
		if(((position.x < (aiPath[0].x + 5.0f)) && (position.x > (aiPath[0].x - 5.0f)))
		&&((position.z < (aiPath[0].z + 5.0f)) && (position.z > (aiPath[0].z - 5.0f))))
		{
			
			float xRand, zRand;
			c++;
			std::random_device rseed;
			std::mt19937 rng(rseed());

			std::uniform_int<int> dist1(xBounds.x, xBounds.y);
			xRand = dist1(rng)*5;
			std::uniform_int<int> dist2(zBounds.x, zBounds.y);
			zRand = dist2(rng)*5;

			lastLoc = aiPath[0];
			aiPath[0] = Vector3(xRand, position.y, zRand);
			direction = aiPath[0] - position;
			D3DXVec3Normalize(&direction, &direction);

			elapsed = 0;
		}

		//Move toward waypoint, updating both position and direction

		position += direction * speed * dt;
		moving = true;
		
		break;
	case PATH:


		//Check if close to waypoint
		//	If so, calculate and set new waypoint
		if(((position.x < (aiPath[pathIndex].x + 5.0f)) && (position.x > (aiPath[pathIndex].x - 5.0f)))
		&&((position.z < (aiPath[pathIndex].z + 5.0f)) && (position.z > (aiPath[pathIndex].z - 5.0f))))
		{
			if(pathIndex == aiPath.size() - 1)
			{
				if (aiPath[aiPath.size() - 1] == aiPath[0])
				{
					pathIndex = -1;
				}
				else
					pathDir = -1;
			}
			if (pathIndex == 0)
			{
				pathDir = 1;
			}
			pathIndex += pathDir;
			if (pathIndex > aiPath.size() - 1)
				pathIndex = aiPath.size() - 1;
			direction = aiPath[pathIndex] - position;
			D3DXVec3Normalize(&direction, &direction);
			

			elapsed = 0;
		}

		//Move toward waypoint, updating position
		direction = aiPath[pathIndex] - position;
		if (aiPath.size() == 1)
		{
			direction = Vector3(0.0f, 0.0f, 1.0f);
			speed = 0;
		}
		D3DXVec3Normalize(&direction, &direction);
		position += direction * speed * dt;
		moving = true;

		break;
	}
	

	//direction.x = sinf(dirTheta);
	//direction.z = cosf(dirTheta);
	torso->setDirection(direction);
	torso->setPosition(Vector3(position.x, position.y + height * 0.5f, position.z));
	
	spotLight->pos = position + Vector3(0.0f, head->getPosition().y, 0.0f) + torso->getDirection() * depth;
	Vector3 lightTarget = position + direction * 20;
	Vector3 lightLookAt = lightTarget - spotLight->pos;
	//spotLight->pos.y += 10.0f;
	//Vector3 normalizedDir = (torso->getDirection()*12)-torso->getPosition();
	D3DXVec3Normalize(&spotLight->dir, &(lightLookAt));
	
	//	leg movement
	float normPos = 175;
	float legRot = sin(elapsed * limbSpeed);
	float phase = 0;
	float shinRot = sin(elapsed * limbSpeed + phase);
	float legRange = 0;
	float legOffset = 0;
	float shinRange = 0;
	float shinOffset = 10;
	if (moving && !sprinting) // set the leg position variables
	{	//swing legs back and forth if moving
		normPos = 175;
		legRot = sin(elapsed * limbSpeed);
		phase = 1.414f;
		shinRot = sin(elapsed * limbSpeed + phase);
		legRange = 20;
		legOffset = 0;
		shinRange = 25;
		shinOffset = 15;
	}
	else if (moving && sprinting)
	{
		normPos = 175;
		legRot = sin(elapsed * limbSpeed);
		phase = 1.414f;
		shinRot = sin(elapsed * limbSpeed + phase);
		legRange = 90;
		legOffset = -45;
		shinRange = 80;
		shinOffset = 55;
	}
	// set the leg positions
	rightLeg->setRotX(ToRadian(normPos + (-legRot * legRange + legOffset)));
	leftLeg->setRotX(ToRadian(normPos + (legRot * legRange + legOffset)));

	rightShin->setRotX(ToRadian(shinRot * shinRange + shinOffset));
	leftShin->setRotX(ToRadian(-shinRot * shinRange + shinOffset));


	//if (keyPressed(PlayerJumpKey))
	//{
	//	torso->reduceScaleByFactor(1.01f);
	//}
	
	//	arm movement
	// rotate arms down
	rightArm->setRotX(ToRadian(180));
	leftArm->setRotX(ToRadian(180));
	if (moving && !sprinting)
	{	//swing arms back and forth if moving
		float normPos = 180;
		float armRot = sin(elapsed * limbSpeed);
		float armRange = 10;
		rightArm->setRotX(ToRadian(normPos + (armRot * armRange)));
		leftArm->setRotX(ToRadian(normPos + (-armRot * armRange)));
	}
	else if (moving && sprinting)
	{
		float normPos = 180;
		float armRot = sin(elapsed * limbSpeed);
		float armRange = 30;
		float armOffset = 10;
		rightArm->setRotX(ToRadian(normPos + (armRot * armRange) + armOffset));
		leftArm->setRotX(ToRadian(normPos + (-armRot * armRange) + armOffset));
	}
	else
	{	//move arms by side if not moving
		rightArm->setRotX(ToRadian(180));
		leftArm->setRotX(ToRadian(180));
	}

	//Update the bodyparts
	head->update(dt);
	rightArm->update(dt);
	leftArm->update(dt);
	rightLeg->update(dt);
	leftLeg->update(dt);
	rightShin->update(dt);
	leftShin->update(dt);
	torso->update(dt);
	
}

void Enemy::draw(Matrix mVP)
{
	if (!active)
		return;
	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		torso->draw(mVP);
    }
}


void Enemy::setDiffuseMap(ID3D10EffectShaderResourceVariable* var)
{
	diffuseMapVar = var;
}

void Enemy::setPosition(Vector3 pos)
{
	position = pos;
}

void Enemy::reset()
{
	spotLight->range = 300.0f;
	setActive();
}