
#ifndef LINE_H
#define LINE_H

#include "d3dUtil.h"
#include "constants.h"
class Line
{
public:

	Line();
	~Line();

	void init(ID3D10Device* device, float scale);
	void init(ID3D10Device* device, float scale, D3DXCOLOR c);
	void draw();
	//void setWorld(Matrix w) {world = w;}
	//Matrix getWorld() {return world;}
	//void update(float dt);
	//void setRotXAngle(float angle) {rotX = angle;}
	//void setRotYAngle(float angle) {rotY = angle;}
	//void setRotZAngle(float angle) {rotZ = angle;}
	//void setPosition(Vector3 pos) {position = pos;}
	//Vector3 getPosition() {return position;}

private:
	DWORD mNumVertices;
	/*DWORD mNumFaces;*/

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	//float rotX, rotY, rotZ;
	//Matrix world;
	//Vector3 position;
};


#endif 
