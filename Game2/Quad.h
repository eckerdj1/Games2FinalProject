
#ifndef QUAD_H
#define QUAD_H

#include "d3dUtil.h"
#include "constants.h"
class Quad
{
public:

	Quad();
	~Quad();

	void init(ID3D10Device* device, float scale);
	void init(ID3D10Device* device, float scale, D3DXCOLOR c);
	void init(ID3D10Device* device, float scaleX, float scaleY, D3DXCOLOR c);
	void draw();
	void setWorld(Matrix w) {world = w;}
	Matrix getWorld() {return world;}
	void update(float dt);
	void setRotXAngle(float angle) {rotX = angle;}
	void setRotYAngle(float angle) {rotY = angle;}
	void setRotZAngle(float angle) {rotZ = angle;}
	void setPosition(Vector3 pos) {position = pos;}
	void setScaleX(float x);
	Vector3 getPosition() {return position;}
	
	void setTexCoords(float top, float left, float bottom, float right);

	float rotX, rotY, rotZ;
private:
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	Color spec;
	Color color;

	Matrix world;
	Vector3 position;

	float scaleX, scaleY;
	
	Vector2 tl;
	Vector2 tr;
	Vector2 bl;
	Vector2 br;
};


#endif 
