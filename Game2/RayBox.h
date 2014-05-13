#pragma once
#include "d3dUtil.h"
#include "constants.h"

// Ray-Bounding box intersection
// http://people.csail.mit.edu/amy/papers/box-jgt.pdf
class Ray 
{
public:
	Ray(Vector3 &o, Vector3 &d) {
		origin = o;
		direction = d;
		inv_direction = Vector3(1/d.x, 1/d.y, 1/d.z);
		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}
	Vector3 origin;
	Vector3 direction;
	Vector3 inv_direction;
	int sign[3];
};

class Box2 {
public:
	Box2(const Vector3 &min, const Vector3 &max) {
		assert(min < max);
		bounds[0] = min;
		bounds[1] = max;
	}
	bool intersect(const Ray &, float t0, float t1);
	Vector3 bounds[2];
};