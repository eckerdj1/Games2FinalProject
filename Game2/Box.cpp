//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Box.h"
#include "Vertex.h"
#include "Player.h"


Box::Box()
: mNumVertices(0), mNumFaces(0), md3dDevice(0), mVB(0), mIB(0)
{
	diffuseMapVar = 0;
	setTexCoords(0.0f, 0.0f, 1.0f, 1.0f);
}
 
Box::~Box()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
void Box::init(ID3D10Device* device, float scale, D3DXCOLOR c)
{
	md3dDevice = device;
 
	mNumVertices = 24;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
	color1 = c;
	color2 = c;
	float specPower, specM;
	specM = 0.7f;
	specPower = 64.0f;
	Color spec = Color(c.r * specM, c.g * specM, c.b * specM, specPower);
	Vector3 up, down, left, right, front, back;
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	right = Vector3(1,0,0);
	left = Vector3(-1,0,0);
	front = Vector3(0,0,-1);
	back = Vector3(0,0,1);
    Vertex vertices[] =
    {
		//front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), front, c, spec}, //left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), front, c, spec},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), front, c, spec},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), front, c, spec},	//right bottom
		//back
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), back, c, spec},	//left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), back, c, spec},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), back, c, spec},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), back, c, spec},	//right bottom
		//right
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), right, c, spec},	//bottom front
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), right, c, spec},	//top front
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), right, c, spec},	//top back
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), right, c, spec},	//bottom back
		//left
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), left, c, spec},	//bottom back
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), left, c, spec},	//top back
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), left, c, spec},	//top front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), left, c, spec},	//bottom front
		//top
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), up, c, spec},	//left front
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), up, c, spec},	//left back
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), up, c, spec},	//right back
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), up, c, spec},	//right front
		//bottom
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), down, c, spec},	//left back
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), down, c, spec},	//left front
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), down, c, spec},	//right front
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), down, c, spec}	//right back
    };

	// Scale the box.
	scaleX = scale;
	scaleY = scale;
	scaleZ = scale;
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face	0-3
		0, 1, 2,
		0, 2, 3,

		// back face	4-7
		4, 6, 5,
		4, 7, 6,

		// right face	8-11
		8, 9, 10,
		8, 10, 11,

		// left face	12-15
		12, 13, 14,
		12, 14, 15,

		// top face		16-19
		16, 17, 18,
		16, 18, 19,

		// bottom face	20-23
		20, 21, 22,
		20, 22, 23
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
void Box::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;
 
	mNumVertices = 8;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
	color1 = White;
	color2 = White;
    Vertex vertices[] =
    {
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), White},
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f), Black},
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f), Red},
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f), Green},
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f), Blue},
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f), Yellow},
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f), Cyan},
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f), Magenta},
    };

	// Scale the box.
	scaleX = scale;
	scaleY = scale;
	scaleZ = scale;
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Box::init(ID3D10Device* device, float sX, float sY, float sZ, D3DXCOLOR c1,D3DXCOLOR c2)
{
	md3dDevice = device;
	
	for (int i=0; i<6; ++i)
	{
		HR(D3DX10CreateShaderResourceViewFromFile(device, 
			L"noTex.jpg", 0, 0, &textures.rv[i], 0 ));
	}
	frontTex.init(device, sX, sY, c1);
	backTex.init(device, sX, sY, c1);
	leftSideTex.init(device, sZ, sY, c1);
	rightSideTex.init(device, sZ, sY, c1);
	topTex.init(device, sX, sZ, c1);
	bottomTex.init(device, sX, sZ, c1);
	
	
	mNumVertices = 24;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
	color1 = c1;
	color2 = c2;
	float specPower, specM;
	specM = 0.7f;
	specPower = 64.0f;
	Color spec1 = Color(c1.r * specM, c1.g * specM, c1.b * specM, specPower);
	Color spec2 = Color(c2.r * specM, c2.g * specM, c2.b * specM, specPower);
	Vector3 up, down, left, right, front, back;
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	right = Vector3(1,0,0);
	left = Vector3(-1,0,0);
	front = Vector3(0,0,-1);
	back = Vector3(0,0,1);

    Vertex vertices[] =
    {
		//front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), front, c1, spec1, bl},	//left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), front, c1, spec1, tl},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), front, c1, spec1, tr},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), front, c1, spec1, br},	//right bottom
		//back
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), back, c1, spec1, br},		//left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), back, c1, spec1, tr},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), back, c1, spec1, tl},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), back, c1, spec1, bl},		//right bottom
		//right
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), right, c1, spec1, bl},	//bottom front
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), right, c1, spec1, tl},	//top front
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), right, c2, spec2, tr},	//top back
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), right, c2, spec2, br},	//bottom back
		//left
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), left, c2, spec2, bl},		//bottom back
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), left, c2, spec2, tl},	//top back
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), left, c1, spec1, tr},	//top front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), left, c1, spec1, br},		//bottom front
		//top
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), up, c1, spec1, bl},		//left front
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), up, c2, spec2, tl},		//left back
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), up, c2, spec2, tr},		//right back
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), up, c1, spec1, br},		//right front
		//bottom
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), down, c2, spec2, bl},		//left back
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), down, c1, spec1, tl},		//left front
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), down, c1, spec1, tr},		//right front
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), down, c2, spec2, br}		//right back
    };

	// Scale the box.
	scaleX = sX;
	scaleY = sY;
	scaleZ = sZ;

	for(DWORD i = 0; i < mNumVertices; ++i) {
		vertices[i].pos.x *= scaleX;
		vertices[i].pos.y *= scaleY;
		vertices[i].pos.z *= scaleZ;
	}


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face	0-3
		0, 1, 2,
		0, 2, 3,

		// back face	4-7
		4, 6, 5,
		4, 7, 6,

		// right face	8-11
		8, 9, 10,
		8, 10, 11,

		// left face	12-15
		12, 13, 14,
		12, 14, 15,

		// top face		16-19
		16, 17, 18,
		16, 18, 19,

		// bottom face	20-23
		20, 21, 22,
		20, 22, 23
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Box::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	/*if (diffuseMapVar)
		diffuseMapVar->SetResource(textures.rv[0]);*/
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
	//if (textured)
	//{
	//	frontTex.
	//}
}


void Box::setVertexColor(Color c1,Color c2) {
	mVB->Release();
	color1 = c1;
	color2 = c2;
	float specPower, specM;
	specM = 0.9f;
	specPower = 64.0f;
	Color spec1 = Color(c1.r * specM, c1.g * specM, c1.b * specM, specPower);
	Color spec2 = Color(c2.r * specM, c2.g * specM, c2.b * specM, specPower);
	Vector3 up, down, left, right, front, back;
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	right = Vector3(1,0,0);
	left = Vector3(-1,0,0);
	front = Vector3(0,0,-1);
	back = Vector3(0,0,1);
    Vertex vertices[] =
    {
		//front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), front, c1, spec1},	//left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), front, c1, spec1},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), front, c1, spec1},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), front, c1, spec1},	//right bottom
		//back
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), back, c1, spec1},	    //left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), back, c1, spec1},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), back, c1, spec1},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), back, c1, spec1},		//right bottom
		//right
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), right, c1, spec1},	//bottom front
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), right, c1, spec1},	//top front
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), right, c2, spec2},	//top back
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), right, c2, spec2},	//bottom back
		//left
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), left, c2, spec2},		//bottom back
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), left, c2, spec2},	//top back
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), left, c1, spec1},	//top front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), left, c1, spec1},		//bottom front
		//top
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), up, c1, spec1},		//left front
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), up, c2, spec2},		//left back
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), up, c2, spec2},		//right back
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), up, c1, spec1},		//right front
		//bottom
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), down, c2, spec2},		//left back
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), down, c1, spec1},		//left front
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), down, c1, spec1},		//right front
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), down, c2, spec2}		//right back
    };

	for(DWORD i = 0; i < mNumVertices; ++i) {
		vertices[i].pos.x *= scaleX;
		vertices[i].pos.y *= scaleY;
		vertices[i].pos.z *= scaleZ;
	}

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}

Color Box::getColor1() {
	return color1;
}
Color Box::getColor2() {
	return color2;
}

void Box::releaseVBuffer()
{
	mVB->Release();
}

void Box::setDiffuseMap(ID3D10EffectShaderResourceVariable* var)
{
	diffuseMapVar = var;
}

void Box::setTexCoords(float top, float left, float bottom, float right)
{
	this->tl = Vector2(left, top);
	this->tr = Vector2(right, top);
	this->bl = Vector2(left, bottom);
	this->br = Vector2(right, bottom);
}

void Box::rescale(float sx, float sy, float sz)
{
	mVB->Release();
	float specPower, specM;
	specM = 0.9f;
	specPower = 64.0f;
	Color spec1 = Color(color1.r * specM, color1.g * specM, color1.b * specM, specPower);
	Vector3 up, down, left, right, front, back;
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	right = Vector3(1,0,0);
	left = Vector3(-1,0,0);
	front = Vector3(0,0,-1);
	back = Vector3(0,0,1);
    Vertex vertices[] =
    {
		//front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), front, color1, spec1, bl},	//left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), front, color1, spec1, tl},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), front, color1, spec1, tr},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), front, color1, spec1, br},	//right bottom
		//back
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), back, color1, spec1, br},		//left bottom
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), back, color1, spec1, tr},	//left top
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), back, color1, spec1, tl},	//right top
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), back, color1, spec1, bl},		//right bottom
		//right
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), right, color1, spec1, bl},	//bottom front
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), right, color1, spec1, tl},	//top front
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), right, color1, spec1, tr},	//top back
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), right, color1, spec1, br},	//bottom back
		//left
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), left, color1, spec1, bl},		//bottom back
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), left, color1, spec1, tl},	//top back
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), left, color1, spec1, tr},	//top front
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), left, color1, spec1, br},		//bottom front
		//top
		{D3DXVECTOR3(-0.5f, +1.0f, -0.5f), up, color1, spec1, bl},		//left front
		{D3DXVECTOR3(-0.5f, +1.0f, +0.5f), up, color1, spec1, tl},		//left back
		{D3DXVECTOR3(+0.5f, +1.0f, +0.5f), up, color1, spec1, tr},		//right back
		{D3DXVECTOR3(+0.5f, +1.0f, -0.5f), up, color1, spec1, br},		//right front
		//bottom
		{D3DXVECTOR3(-0.5f, 0.0f, +0.5f), down, color1, spec1, bl},		//left back
		{D3DXVECTOR3(-0.5f, 0.0f, -0.5f), down, color1, spec1, tl},		//left front
		{D3DXVECTOR3(+0.5f, 0.0f, -0.5f), down, color1, spec1, tr},		//right front
		{D3DXVECTOR3(+0.5f, 0.0f, +0.5f), down, color1, spec1, br}		//right back
    };
	
	// Scale the box.
	scaleX = sx;
	scaleY = sy;
	scaleZ = sz;

	for(DWORD i = 0; i < mNumVertices; ++i) {
		vertices[i].pos.x *= scaleX;
		vertices[i].pos.y *= scaleY;
		vertices[i].pos.z *= scaleZ;
	}

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}