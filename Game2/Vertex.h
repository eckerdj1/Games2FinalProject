

#ifndef VERTEX_H
#define VERTEX_H

//#include "d3dUtil.h"
#include "d3dUtil.h"

struct Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXCOLOR   diffuse;
	D3DXCOLOR   spec; // (r, g, b, specPower);
	D3DXVECTOR2 texC;
};
 
#endif // VERTEX_H