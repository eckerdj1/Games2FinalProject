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
	direction = Vector3(0,0,0);
}

void Weapon::buildBody()
{


	
}

void Weapon::setRotX(string partName, float rot)
{
	for (int i=0; i<parts.size(); ++i)
	{
		if (parts[i]->getName() == partName)
		{
			parts[i]->setRotX(rot);
		}
	}
}
void Weapon::setRotY(string partName, float rot)
{
	for (int i=0; i<parts.size(); ++i)
	{
		if (parts[i]->getName() == partName)
		{
			parts[i]->setRotY(rot);
		}
	}
}
void Weapon::setRotZ(string partName, float rot)
{
	for (int i=0; i<parts.size(); ++i)
	{
		if (parts[i]->getName() == partName)
		{
			parts[i]->setRotZ(rot);
		}
	}
}

void Weapon::addPart(string n, string addTo, Vector3 pos, Vector3 size)
{
	BodyPart* part = new BodyPart();
	part->init(n, box, pos, direction, size, speed);
	part->setBody(this);
	addToPart(part, addTo);
	parts.push_back(part);
}

void Weapon::addToPart(BodyPart* part, string root)
{
	for (int i=0; i<parts.size(); ++i)
	{
		if (parts[i]->getName() == root)
		{
			part->setRoot(parts[i]);
			parts[i]->addChild(part);
		}
	}
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