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
using std::vector;
using std::string;



class Pickup : public GameObject
{
public:
	Pickup();

	void init(Box *b, Vector3 pos, Vector3 dir, Vector3 size, float speed);
	void init(ID3D10Device* device, Vector3 pos, Vector3 size, D3DXCOLOR c);

	void setTransX(float x) {transX = x;}
	void setTransY(float y) {transY = y;}
	void setTransZ(float z) {transZ = z;}
	
	void setScaleX(float x) {scaleX = x;}
	void setScaleY(float y) {scaleY = y;}
	void setScaleZ(float z) {scaleZ = z;}

	void reduceScaleByFactor(float r);
	
	void setRotX(float rho) {rotX = rho;}
	void setRotY(float theta) {rotY = theta;}
	void setRotZ(float phi) {rotZ = phi;}

	void addRotX(float rho) {rotX += rho;}
	void addRotY(float theta) {rotY += theta;}
	void addRotZ(float phi) {rotZ += phi;}

	Matrix getWorld();

	void update(float dt);

private:

	Matrix world;
	ID3D10Device* device;
	int height;
	float transX, transY, transZ;
	float scaleX, scaleY, scaleZ;
	float rotX, rotY, rotZ;
};