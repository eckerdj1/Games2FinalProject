#include "Pickup.h"

Pickup::Pickup()
{
	GameObject();
	scaleX = scaleY = 1.0f;
	scaleZ = 7.0f;
	rotX = rotY = rotZ = 0.0f;
	transX = transY = transZ = 0.0f;
}

void Pickup::init(Box* b, Vector3 pos, Vector3 dir, Vector3 size, float speed)
{
	GameObject::init(b, pos, dir, size, speed);
}

void Pickup::init(ID3D10Device* device, Vector3 pos, Vector3 size, D3DXCOLOR c) {
	
	Pickup::device = device;
	Box* b = new Box();
	b->init(device, size.x, size.y, size.z, c, c);
	GameObject::init(b, pos, Vector3(0,0,0), size, 0);
	scaleX = size.x;
	scaleY = size.y;
	scaleZ = size.z;
	transX = pos.x;
	transY = pos.y;
	transZ = pos.z;
}

void Pickup::update(float dt)
{
	GameObject::update(dt);
	Identity(&world);
	//transX = position.x;
	//transY = position.y;
	//transZ = position.z;
	Matrix scale;
	Matrix trans;
	Identity(&scale);
	Identity(&trans);
	//Scale(&scale, getSize().x, getSize().y, getSize().z);
	Scale(&scale, scaleX, scaleY, scaleZ);
	Translate(&trans, transX, transY, transZ);
	world = scale * trans * world;
}

Matrix Pickup::getWorld()
{
	return world;
}

void Pickup::reduceScaleByFactor(float r)
{
	scaleX /= r;
	scaleY /= r;
	scaleZ /= r;
}