#include "Weapon.h"


Weapon::Weapon()
{
}

Weapon::~Weapon()
{
	for (int i=0; i<parts.size(); ++i)
		delete parts[i];
}

void Weapon::init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* _device)
{
	name = n;
	position = pos;
	speed = spd;
	size.x = width;
	size.y = height;
	size.z = depth;
	device = _device;

	buildBody();
}

void Weapon::buildBody()
{
	BodyPart* part = new BodyPart();

	part->init("Body", box, position, direction, size, speed);
	part->setBody(this);
	parts.push_back(part);

	part = new BodyPart();
	part->init("FrontHandle", box, Vector3(0.0f, size.y * 0.5f, size.z * 0.25f), direction, 
		Vector3(size.x * 1.87f, size.y * 0.25f, size.y * 0.25f), speed);
	part->setBody(this);
	parts.push_back(part);

	part = new BodyPart();
	part->init("Barrel1", box, Vector3(0.0f, size.y * 0.5f, size.z * 0.55f), direction,
		Vector3(size.x * 0.6f, size.y * 0.8f, size.z * 0.2f), speed);
	part->setBody(this);
	parts.push_back(part);

	
}

void Weapon::attachBox(Box* b)
{
	box = b;
}

void Weapon::update(float dt)
{
	parts[0]->setPosition(position);
	parts[0]->setDirection(direction);
	for (int i=parts.size() - 1; i >= 0; --i)
	{
		parts[i]->update(dt);
	}
}

void Weapon::draw(Matrix mVP)
{
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc( &techDesc );
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		parts[0]->draw(mVP);
	}
}

void Weapon::setDiffuseMap(ID3D10EffectShaderResourceVariable* v)
{
	diffuseMapVar = v;
}

void Weapon::setEffectVariables(ID3D10EffectMatrixVariable* wvpVar, ID3D10EffectMatrixVariable* worldVar)
{
	mfxWVPVar = wvpVar;
	mfxWorldVar = worldVar;
}