#ifndef TILEOBJECT_H
#define TILEOBJECT_H
#include "GameObject.h"

class TileObject : public GameObject {

public:
	
	DXColor color1, color2;

	void changeColors(DXColor c1, DXColor c2);



};

#endif