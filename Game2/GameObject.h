
#ifndef GameObject_H
#define GameObject_H

//#include "d3dUtil.h"
#include "d3dUtil.h"
#include "Box.h"
#include "constants.h"
#include <math.h>
#include <vector>
using std::vector;

class GameObject
{
public:

	GameObject();
	~GameObject();

	//Initialization
	void init(Box *b, Vector3 pos, Vector3 dir, Vector3 size, float speed);

	// Update and draww
	virtual void draw();
	virtual void update(float dt);

	//Getters and Setters
	void setBox(Box* b) {box = b;}
	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}

	void setDirection (Vector3 dir) {direction = dir;}
	Vector3 getDirection() {return direction;}

	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}

	void setRadii(Vector3 r) {radii = r;}
	void setRadii(float x, float y, float z) {radii = Vector3(x,y,z);}
	Vector3 getRadii() {return radii;}

	void setSize(Vector3 s) {size = s;}
	Vector3 getSize() {return size;}

	void setActive() {active = true;}
	void setInActive() {active = false;}

	bool isActive() {return active;}
	bool isNotActive() {return !active;}

	//Object interaction
	bool onTopOf(GameObject *gameObject);
	bool isAbove(GameObject *gameObject);

	void normlizeDirection();
	float getBoxBottom();
	void deleteBox();

	bool contains(Vector3);

	Color colorAtPoint(float zPos);

	float xRadius();
	float yRadius();
	float zRadius();
	Vector3 cornerAt(int i);

	//DirectX Drawing stuff
	Matrix getWorldMatrix() {return world;}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
protected:
	Box *box;
	Matrix world;
	Vector3 position;
	Vector3 direction;

private:
	Vector3 size;
	vector<Vector3> corners;
	float speed;
	Vector3 radii;

	bool active;
	ID3D10EffectTechnique* mTech;
};


#endif
