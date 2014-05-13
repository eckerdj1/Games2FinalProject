#pragma once
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle(void);
	~Obstacle(void);
	
	void update(float dt);
	void setColor(Color color);

private:
	int count;
};

