
#include "Line.h"
#include "Vertex.h"
#include "constants.h"

Line::Line()
: mNumVertices(0), md3dDevice(0), mVB(0), mIB(0)
{
}
 
Line::~Line()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
void Line::init(ID3D10Device* device, float scale, D3DXCOLOR c)
{
	md3dDevice = device;
	mNumVertices = 2;

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), c},
		{D3DXVECTOR3(1.0f, 0.0f, 0.0f), c}
    };

	// Scale the Line.
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
}


void Line::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	md3dDevice->Draw(2,0);
}