// ProcCubeMesh.cpp
// Josh Hale - 2017
// Generates cuboids with the top surface aligned with noise, for noise on a grid

#include "ProcCubeMesh.h"

// Constructor
/// PARAMETERS
// device, deviceContext - pointer to the D3D device information
// textureFilename - the location of the relevant texture file
// cellX, cellZ - the cell location in the grid
// xSegments, zSegments - how large the grid is
// gap - the gap between the edge of the mesh and the edge of the cell, to create gridlines
// noise - parameters to be passed to the noise engine
// noiseEngine - handles noise calculations
ProcCubeMesh::ProcCubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int cellX, int cellZ, int xSegments, int zSegments, float gap, NoiseData* noise, ImprovedNoise* noiseEngine)
{
	// Store calculation variables
	x = cellX;
	z = cellZ;
	xSegs = xSegments;
	zSegs = zSegments;
	m_Noise = noise;
	perlin = noiseEngine;
	border = gap;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitBuffers(device);
}

ProcCubeMesh::~ProcCubeMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Calculate mesh vertices and store in buffers
/// PARAMETERS
// device - pointer to the D3D device
void ProcCubeMesh::InitBuffers(ID3D11Device* device)
{
	// Declare local variables
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Calculate vertex count
	// 6 vertices per quad, times 6 for each face (- 6 for non-visible bottom face)
	// Set the number of vertices in the vertex array.
	m_vertexCount = 30;

	// If needed this only draws the top face to lower poly count
	//if (x < (xSegs - 1))
	//{
	//	// don't create right face
	//	m_vertexCount -= 6;
	//}
	//if (x > 0)
	//{
	//	// don't create left face
	//	m_vertexCount -= 6;
	//}
	//if (z < (zSegs - 1))
	//{
	//	// don't create back face
	//	m_vertexCount -= 6;
	//}
	//if (z > 0)
	//{
	//	// don't create front face
	//	m_vertexCount -= 6;
	//}

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	// Calculate scale of the top face of the cube 
	float size = 1.0f - border;
	float nSize = 0.5f * size * m_Noise->scaleModifier;

	// Calculate height of the top face vertices for the cube
	float height010 = GetHeight(x - nSize, z - nSize);
	float height011 = GetHeight(x - nSize, z + nSize);
	float height110 = GetHeight(x + nSize, z - nSize);
	float height111 = GetHeight(x + nSize, z + nSize);

	// Calculate vertex positions of the cube
	XMFLOAT3 vertex000 = XMFLOAT3(-size, -size, -size);
	XMFLOAT3 vertex100 = XMFLOAT3(size, -size, -size);
	XMFLOAT3 vertex010 = XMFLOAT3(-size, height010 * m_Noise->Height, -size);
	XMFLOAT3 vertex110 = XMFLOAT3(size, height110 * m_Noise->Height, -size);
	XMFLOAT3 vertex001 = XMFLOAT3(-size, -size, size);
	XMFLOAT3 vertex101 = XMFLOAT3(size, -size, size);
	XMFLOAT3 vertex011 = XMFLOAT3(-size, height011 * m_Noise->Height, size);
	XMFLOAT3 vertex111 = XMFLOAT3(size, height111 * m_Noise->Height, size);

	// Calculate and set normals
	XMFLOAT3 up0 = Cross(Subtract(vertex011, vertex010), Subtract(vertex111, vertex011));
	XMFLOAT3 up1 = Cross(Subtract(vertex111, vertex110), Subtract(vertex110, vertex010));
	XMFLOAT3 down = XMFLOAT3(0, -1, 0);
	XMFLOAT3 left = XMFLOAT3(-1, 0, 0);
	XMFLOAT3 right = XMFLOAT3(1, 0, 0);
	XMFLOAT3 front = XMFLOAT3(0, 0, -1);
	XMFLOAT3 back = XMFLOAT3(0, 0, 1);

	// Set texture coordinates
	XMFLOAT2 tx00 = XMFLOAT2(0, 0);
	XMFLOAT2 tx01 = XMFLOAT2(0, 1);
	XMFLOAT2 tx10 = XMFLOAT2(1, 0);
	XMFLOAT2 tx11 = XMFLOAT2(1, 1);

	// Assign vertex positions, normals and texture coordinates to correct vertices
	int i = 0;
	XMFLOAT3 normal = front;
	//if (!(z > 0))
	//{
		// Front
		vertices[i].position = vertex000;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex110;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
		vertices[i].position = vertex010;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex000;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex100;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex110;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
	//}
	//if (!(x < (xSegs - 1)))
	//{
		// Right
		normal = right;
		vertices[i].position = vertex100;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex111;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
		vertices[i].position = vertex110;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex100;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex101;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex111;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
	//}
	//if (!(z < (zSegs - 1)))
	//{
		// Back
		normal = back;
		vertices[i].position = vertex101;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex011;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
		vertices[i].position = vertex111;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex101;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex001;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex011;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
	//}
	//if (!(x > 0))
	//{
		// Left
		normal = left;
		vertices[i].position = vertex001;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex010;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
		vertices[i].position = vertex011;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex001;
		vertices[i].normal = normal;
		vertices[i].texture = tx00;
		indices[i] = i;
		i++;
		vertices[i].position = vertex000;
		vertices[i].normal = normal;
		vertices[i].texture = tx10;
		indices[i] = i;
		i++;
		vertices[i].position = vertex010;
		vertices[i].normal = normal;
		vertices[i].texture = tx11;
		indices[i] = i;
		i++;
//	}
	// Top
	//normal = up;
	vertices[i].position = vertex010;
	vertices[i].normal = up0;
	vertices[i].texture = tx00;
	indices[i] = i;
	i++;
	vertices[i].position = vertex111;
	vertices[i].normal = up0;
	vertices[i].texture = tx11;
	indices[i] = i;
	i++;
	vertices[i].position = vertex011;
	vertices[i].normal = up0;
	vertices[i].texture = tx10;
	indices[i] = i;
	i++;
	vertices[i].position = vertex010;
	vertices[i].normal = up1;
	vertices[i].texture = tx00;
	indices[i] = i;
	i++;
	vertices[i].position = vertex110;
	vertices[i].normal = up1;
	vertices[i].texture = tx10;
	indices[i] = i;
	i++;
	vertices[i].position = vertex111;
	vertices[i].normal = up1;
	vertices[i].texture = tx11;
	indices[i] = i;
	i++;
	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

// Returns a value of the noise at the point sampled
/// PARAMETERS
// xPos, zPos - vertex position in the x- and z-axis for sampling a location in noise
/// OUTPUT
// float - 0 < n < 1 value representing the noise at the location sampled
float ProcCubeMesh::GetHeight(float xPos, float zPos)
{
	float noiseX = m_Noise->Resolution * xPos / xSegs + m_Noise->Location.x;
	float noiseZ = m_Noise->Resolution * zPos / zSegs + m_Noise->Location.y;
	return (float)perlin->noise(noiseX, 0.0f, noiseZ);
}

// Returns the cross product of two vectors stored as XMFLOAT3's
/// PARAMETERS
// a, b - vectors to be crossed
/// OUTPUT
// XMFLOAT3 - the result of the cross product
XMFLOAT3 ProcCubeMesh::Cross(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 c = XMFLOAT3();
	c.x = (a.y*b.z) - (a.z*b.y);
	c.y = (a.z*b.x) - (a.x*b.z);
	c.z = (a.x*b.y) - (a.y*b.x);
	return c;
}

// Returns the result of a vector subtraction of two XMFLOAT3's
/// PARAMETERS
// a, b - vector b is subtracted from vector a
/// OUTPUT
// XMFLOAT3 - the result of the subtraction
XMFLOAT3 ProcCubeMesh::Subtract(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 c = XMFLOAT3();
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	XMStoreFloat3(&c, XMVector3Normalize(XMLoadFloat3(&c)));

	return c;
}



