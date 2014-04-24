#include "Wall.h"

Wall::Wall()
{
	GameObject();
	scaleX = scaleY = 1.0f;
	scaleZ = 7.0f;
	rotX = rotY = rotZ = 0.0f;
	transX = transY = transZ = 0.0f;
}

void Wall::init(Box* b, Vector3 pos, Vector3 dir, Vector3 size, float speed)
{
	GameObject::init(b, pos, dir, size, speed);
}

void Wall::init(ID3D10Device* device, Vector3 pos, Vector3 size, D3DXCOLOR c) {
	
	Wall::device = device;
	float width = Max(size.x, size.z) / 14.5f;
	b->setTexCoords(0.0f, 0.0f, 1.0f, width);
	b->init(device, size.x, size.y, size.z, c, c);
	GameObject::init(b, pos, Vector3(0,0,0), size, 0);
	scaleX = size.x;
	scaleY = size.y;
	scaleZ = size.z;
	transX = pos.x;
	transY = pos.y;
	transZ = pos.z;
}

void Wall::update(float dt)
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

Matrix Wall::getWorld()
{
	return world;
}

void Wall::reduceScaleByFactor(float r)
{
	scaleX /= r;
	scaleY /= r;
	scaleZ /= r;
}

void Wall::attachBox(Box* box)
{
	b = box;
}

void Wall::draw()
{
	if (isNotActive())
		return;
	
	float width = Max(getSize().x, getSize().z) / 14.5f;
	b->setTexCoords(0.0f, 0.0f, 1.0f, width);
	box->rescale(scaleX, scaleY, scaleZ);
	
	GameObject::draw();
}