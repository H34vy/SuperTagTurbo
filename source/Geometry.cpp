//=======================================================================================
// Geometry.cpp
// Chris Prosser.
//		Modified from Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Geometry.h"
#include "Vertex.h"

Geometry::Geometry()
: mNumVertices(0), mNumFaces(0), device(0), mVB(0), mIB(0)
{
}
 
Geometry::~Geometry()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}


void Box::init(Device* d, float scale, Color c)
{
	device = d;
	collidable = true;
	mNumVertices = 8;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
    Vertex vertices[] =
    {
		{Vector3(-1.0f, -1.0f, -1.0f), c},
		{Vector3(-1.0f, +1.0f, -1.0f), c},
		{Vector3(+1.0f, +1.0f, -1.0f), c},
		{Vector3(+1.0f, -1.0f, -1.0f), c},
		{Vector3(-1.0f, -1.0f, +1.0f), c},
		{Vector3(-1.0f, +1.0f, +1.0f), c},
		{Vector3(+1.0f, +1.0f, +1.0f), c},
		{Vector3(+1.0f, -1.0f, +1.0f), c},
    };

	// Scale the Geometry.
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
    HR(device->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Box::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    device->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	device->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	device->DrawIndexed(mNumFaces*3, 0, 0);
}

void RainbowBox::init(Device* d, float scale)
{
	device = d;
	collidable = true;
	mNumVertices = 8;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
    Vertex vertices[] =
    {
		{Vector3(-1.0f, -1.0f, -1.0f), WHITE},
		{Vector3(-1.0f, +1.0f, -1.0f), BLACK},
		{Vector3(+1.0f, +1.0f, -1.0f), RED},
		{Vector3(+1.0f, -1.0f, -1.0f), GREEN},
		{Vector3(-1.0f, -1.0f, +1.0f), BLUE},
		{Vector3(-1.0f, +1.0f, +1.0f), YELLOW},
		{Vector3(+1.0f, +1.0f, +1.0f), CYAN},
		{Vector3(+1.0f, -1.0f, +1.0f), MAGENTA},
    };

	// Scale the Geometry.
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
    HR(device->CreateBuffer(&ibd, &iinitData, &mIB));
}

void RainbowBox::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    device->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	device->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	device->DrawIndexed(mNumFaces*3, 0, 0);
}


void Line::init(Device* d, float scale, Color c)
{
	device = d;
 	collidable = false;
	mNumVertices = 2;	
	
	// Create vertex buffer
    Vertex vertices[] =
    {
		{Vector3(0.f, 0.f, 0.f), c},
		{Vector3(1.f, 0.f, 0.f), c},		
    };

	// Scale the line
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

void Pyramid::init(Device* d, float scale, Color c) {
	device = d;
	collidable = false;
	mNumVertices = 5;
	mNumFaces = 5;

	Vertex vertices[] =
	{
		{Vector3(0.f,1.f,0.f),c},
		{Vector3(1.f,0.f,0.f),c},
		{Vector3(0.f,0.f,1.f),c},
		{Vector3(-1.f,0.f,0.f),c},
		{Vector3(0.f,0.f,-1.f),c}
	};

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(device->CreateBuffer(&vbd, &vinitData, &mVB));

	// Create the index buffer

	DWORD indices[] = {
		0,1,2,

		0,2,3,

		0,3,4,

		0,4,1,

		1,2,3,

		1,3,4
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(device->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Pyramid::draw() {
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    device->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	device->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	device->DrawIndexed(mNumFaces*3, 0, 0);
}