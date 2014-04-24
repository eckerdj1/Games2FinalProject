#pragma once
#include "Box.h"
#include "GameObject.h"
#include "constants.h"
#include "d3dUtil.h"
#include "input.h"
#include <math.h>
#include <vector>
#include <string>
#include "audio.h"

class Floor : public GameObject
{
public:
	Floor();
	~Floor();

	void init(ID3D10Device* m3dDevice, float xSize, float ySize);
	//void draw(float dt);

	void update(float dt);

private:
	Vector2 dimensions;

	ID3D10Device* device;
};