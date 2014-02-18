#include "Line.h"
#include "Vertex.h"

void Line::init(Device* d, float scale, Color c)
{
	device = d;
 	mNumVertices = 2;
	
	// Create vertex buffer
    Vertex vertices[] =
    {
		{Vector3(0.f, 0.f, 0.f), c},
		{Vector3(1.f, 0.f, 0.f), c},		
    };

	// Scale the box.
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
    HR(device->CreateBuffer(&vbd, &vinitData, &mVB));
	
	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD);
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
}

void Line::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
    device->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	device->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	device->Draw(2,0); 
}