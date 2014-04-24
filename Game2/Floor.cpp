#include "Floor.h"

Floor::Floor()
{
	dimensions = Vector2(1, 1);
}

Floor::~Floor()
{

}

void Floor::init(ID3D10Device* m3dDevice, float xSize, float ySize)
{
	device = m3dDevice;
	Box* b = new Box();
	b->setTexCoords(0.0f, 0.0f, xSize / 100.0f, ySize / 100.0f);
	b->init(device, 1, 1, 1, White, White);
	GameObject::init(b, Vector3(0,0,0), Vector3(0,0,0), Vector3(xSize, 0.01f, ySize) , 0);

	dimensions.x = xSize;
	dimensions.y = ySize;
}

void Floor::update(float dt)
{
	Identity(&world);
	Matrix scale;
	Identity(&scale);
	Scale(&scale, getSize().x, getSize().y, getSize().z);
	world = scale * world;
}

//void Floor::draw(float dt)
//{
//
//}