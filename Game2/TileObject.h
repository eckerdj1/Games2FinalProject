#ifndef TILEOBJECT_H
#define TILEOBJECT_H
#include "GameObject.h"

class TileObject : public GameObject {

public:
	
	Color color1, color2;

	void changeColors(Color c1, Color c2);



};

#endif