#include "RayBox.h"

// Optimized method
bool Box2::intersect(const Ray &r, float t0, float t1){
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = (bounds[r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tmax = (bounds[1-r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tymin = (bounds[r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	tymax = (bounds[1-r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	if ( (tmin > tymax) || (tymin > tmax) )
	return false;
	if (tymin > tmin)
	tmin = tymin;
	if (tymax < tmax)
	tmax = tymax;
	tzmin = (bounds[r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	tzmax = (bounds[1-r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ( (tmin < t1) && (tmax > t0) );
}