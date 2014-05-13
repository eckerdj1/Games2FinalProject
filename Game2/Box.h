//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef BOX_H
#define BOX_H

#include "d3dUtil.h"
#include "constants.h"
#include "CubeTex.h"
#include "Quad.h"

class Player;

class Box
{
public:

	Box();
	~Box();

	void init(ID3D10Device* device, float scale);
	void init(ID3D10Device* device, float scale, D3DXCOLOR c);
	void init(ID3D10Device* device, float sX, float sY, float sZ, D3DXCOLOR c1, D3DXCOLOR c2);
	void draw();
	void releaseVBuffer();
	
	void setDiffuseMap(ID3D10EffectShaderResourceVariable* var);
	void rescale(float sx, float sy, float sz);

	void setVertexColor(Color c1,Color c2);
	Color getColor1();
	Color getColor2();

	void setTexCoords(float top, float left, float bottom, float right);

private:
	DWORD mNumVertices;
	DWORD mNumFaces;
	
	CubeTex textures;
	bool textured;

	float scaleX, scaleY, scaleZ;
	Color color1, color2;

	Quad frontTex, backTex, leftSideTex, rightSideTex, topTex, bottomTex;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	Vector2 tl;
	Vector2 tr;
	Vector2 bl;
	Vector2 br;

	ID3D10EffectShaderResourceVariable* diffuseMapVar;
};


#endif // BOX_H
