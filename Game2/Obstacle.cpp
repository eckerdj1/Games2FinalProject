#include "Obstacle.h"


Obstacle::Obstacle(void)
{
}


Obstacle::~Obstacle(void)
{
	GameObject::~GameObject();
}

void Obstacle::update(float dt) {

	GameObject::update(dt);

	Vector3 pos = GameObject::getPosition();
	if (pos.z < -50)
	{
		setInActive();
	}
}

void Obstacle::setColor(DXColor color)
{
	box->releaseVBuffer();
	box->setVertexColor(color, color);
}
