#include "ProcCubeMesh.h"

// Cube Mesh
// Generates a cube.
//#include "cubemesh.h"

ProcCubeMesh::ProcCubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int cellX, int cellZ, int xSegments, int zSegments, NoiseData* noise, ImprovedNoise* noiseEngine)
{
	x = cellX;
	z = cellZ;
	xSegs = xSegments;
	zSegs = zSegments;
	m_Noise = noise;
	perlin = noiseEngine;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitBuffers(device);

	// Load the texture for this model.
	//LoadTexture(device, deviceContext, textureFilename);
}


ProcCubeMesh::~ProcCubeMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}
/*unity implementation
void ProcCubeMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	// Set the gap between cells (for gridlines)
	float border = 0.0f;
	// Calculate vertex count
	// 6 vertices per quad, times 6 for each face
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6 * 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	// Calculate height of the top face vertices for the cube
	float height100 = GetHeight(x - border, z - border);
	float height101 = GetHeight(x - border, z + border);
	float height110 = GetHeight(x + border, z - border);
	float height111 = GetHeight(x + border, z + border);

	// Calculate vertex positions of the cube
	XMVECTOR vertex100 = { (1 - border), height100 * m_Noise->Height, (1 - border) };
	XMVECTOR vertex101 = { (1 - border), height101 * m_Noise->Height, (border - 1) };
	XMVECTOR vertex110 = { (border - 1), height110 * m_Noise->Height, (1 - border) };
	XMVECTOR vertex111 = { (border - 1), height111 * m_Noise->Height, (border - 1) };
	XMVECTOR vertex000 = { (1 - border), -1.0f, (1 - border) };
	XMVECTOR vertex001 = { (1 - border), -1.0f, (border - 1) };
	XMVECTOR vertex010 = { (border - 1), -1.0f, (1 - border) };
	XMVECTOR vertex011 = { (border - 1), -1.0f, (border - 1) };

	// Assign vertex positions to correct vertices
	XMStoreFloat3(&vertices[0].position, vertex100);//front
	XMStoreFloat3(&vertices[1].position, vertex101);
	XMStoreFloat3(&vertices[2].position, vertex111);
	XMStoreFloat3(&vertices[3].position, vertex100);
	XMStoreFloat3(&vertices[4].position, vertex111);
	XMStoreFloat3(&vertices[5].position, vertex110);
	XMStoreFloat3(&vertices[6].position, vertex110);//back
	XMStoreFloat3(&vertices[7].position, vertex111);
	XMStoreFloat3(&vertices[8].position, vertex011);
	XMStoreFloat3(&vertices[9].position, vertex110);
	XMStoreFloat3(&vertices[10].position, vertex011);
	XMStoreFloat3(&vertices[11].position, vertex010);
	XMStoreFloat3(&vertices[12].position, vertex100);//right
	XMStoreFloat3(&vertices[13].position, vertex110);
	XMStoreFloat3(&vertices[14].position, vertex010);
	XMStoreFloat3(&vertices[15].position, vertex100);
	XMStoreFloat3(&vertices[16].position, vertex010);
	XMStoreFloat3(&vertices[17].position, vertex000);
	XMStoreFloat3(&vertices[18].position, vertex101);//left
	XMStoreFloat3(&vertices[19].position, vertex100);
	XMStoreFloat3(&vertices[20].position, vertex000);
	XMStoreFloat3(&vertices[21].position, vertex101);
	XMStoreFloat3(&vertices[22].position, vertex000);
	XMStoreFloat3(&vertices[23].position, vertex001);
	XMStoreFloat3(&vertices[24].position, vertex111);//top
	XMStoreFloat3(&vertices[25].position, vertex101);
	XMStoreFloat3(&vertices[26].position, vertex001);
	XMStoreFloat3(&vertices[27].position, vertex111);
	XMStoreFloat3(&vertices[28].position, vertex001);
	XMStoreFloat3(&vertices[29].position, vertex011);
	XMStoreFloat3(&vertices[30].position, vertex010);//bottom
	XMStoreFloat3(&vertices[31].position, vertex011);
	XMStoreFloat3(&vertices[32].position, vertex001);
	XMStoreFloat3(&vertices[33].position, vertex010);
	XMStoreFloat3(&vertices[34].position, vertex001);
	XMStoreFloat3(&vertices[35].position, vertex000);

	// Assign all vertices to point to centre of the texture
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].texture = XMFLOAT2(0.5f, 0.5f);
	}

	// Calculate normals
	XMVECTOR normal000111 = XMVector3Normalize(XMVector3Cross(vertex101 - vertex100, vertex111 - vertex100));
	XMVECTOR normal001011 = XMVector3Normalize(XMVector3Cross(vertex111 - vertex100, vertex110 - vertex100));
	XMVECTOR up = { 0,1,0 };
	XMVECTOR normalRight = { 1.0f, 0.0f, 0.0f };
	XMVECTOR normalLeft = { -1.0f, 0.0f, 0.0f };
	XMVECTOR normalFront = { 0.0f, 0.0f, -1.0f };
	XMVECTOR normalBack = { 0.0f, 0.0f, 1.0f };
	XMVECTOR normalBottom = { 0.0f, -1.0f, 0.0f };

	// Assign normals to vertices
	XMStoreFloat3(&vertices[0].normal, up);
	XMStoreFloat3(&vertices[1].normal, up);
	XMStoreFloat3(&vertices[2].normal, up);
	XMStoreFloat3(&vertices[3].normal, up);
	XMStoreFloat3(&vertices[4].normal, up);
	XMStoreFloat3(&vertices[5].normal, up);
	XMStoreFloat3(&vertices[6].normal, normalRight);
	XMStoreFloat3(&vertices[7].normal, normalRight);
	XMStoreFloat3(&vertices[8].normal, normalRight);
	XMStoreFloat3(&vertices[9].normal, normalRight);
	XMStoreFloat3(&vertices[10].normal, normalRight);
	XMStoreFloat3(&vertices[11].normal, normalRight);
	XMStoreFloat3(&vertices[12].normal, normalFront);
	XMStoreFloat3(&vertices[13].normal, normalFront);
	XMStoreFloat3(&vertices[14].normal, normalFront);
	XMStoreFloat3(&vertices[15].normal, normalFront);
	XMStoreFloat3(&vertices[16].normal, normalFront);
	XMStoreFloat3(&vertices[17].normal, normalFront);
	XMStoreFloat3(&vertices[18].normal, normalLeft);
	XMStoreFloat3(&vertices[19].normal, normalLeft);
	XMStoreFloat3(&vertices[20].normal, normalLeft);
	XMStoreFloat3(&vertices[21].normal, normalLeft);
	XMStoreFloat3(&vertices[22].normal, normalLeft);
	XMStoreFloat3(&vertices[23].normal, normalLeft);
	XMStoreFloat3(&vertices[24].normal, normalBack);
	XMStoreFloat3(&vertices[25].normal, normalBack);
	XMStoreFloat3(&vertices[26].normal, normalBack);
	XMStoreFloat3(&vertices[27].normal, normalBack);
	XMStoreFloat3(&vertices[28].normal, normalBack);
	XMStoreFloat3(&vertices[29].normal, normalBack);
	XMStoreFloat3(&vertices[30].normal, normalBottom);
	XMStoreFloat3(&vertices[31].normal, normalBottom);
	XMStoreFloat3(&vertices[32].normal, normalBottom);
	XMStoreFloat3(&vertices[33].normal, normalBottom);
	XMStoreFloat3(&vertices[34].normal, normalBottom);
	XMStoreFloat3(&vertices[35].normal, normalBottom);

	// Assign indices
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

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
}*/


void ProcCubeMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	// Set the gap between cells (for gridlines)
	float border = 0.05f;
	// Calculate vertex count
	// 6 vertices per quad, times 6 for each face
	// Set the number of vertices in the vertex array.
	m_vertexCount = 30;

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

	float size = 1.0f - border;
	float nSize = 0.5f * size * m_Noise->scaleModifier;
	// Calculate height of the top face vertices for the cube
	float height010 = GetHeight(x - nSize, z - nSize);
	float height011 = GetHeight(x - nSize, z + nSize);
	float height110 = GetHeight(x + nSize, z - nSize);
	float height111 = GetHeight(x + nSize, z + nSize);

	//height010 = 0.3f;
	//height011 = 0.1f;
	//height110 = 0.6f;
	//height111 = 0.2f;

	// Calculate vertex positions of the cube
	XMFLOAT3 vertex000 = XMFLOAT3(-size, -size, -size);
	XMFLOAT3 vertex100 = XMFLOAT3(size, -size, -size);
	XMFLOAT3 vertex010 = XMFLOAT3(-size, height010, -size);
	XMFLOAT3 vertex110 = XMFLOAT3(size, height110, -size);
	XMFLOAT3 vertex001 = XMFLOAT3(-size, -size, size);
	XMFLOAT3 vertex101 = XMFLOAT3(size, -size, size);
	XMFLOAT3 vertex011 = XMFLOAT3(-size, height011, size);
	XMFLOAT3 vertex111 = XMFLOAT3(size, height111, size);

	XMFLOAT3 up0 = Cross(Subtract(vertex011, vertex010), Subtract(vertex111, vertex011));
	XMFLOAT3 up1 = Cross(Subtract(vertex111, vertex110), Subtract(vertex110, vertex010));
	XMFLOAT3 down = XMFLOAT3(0, -1, 0);
	XMFLOAT3 left = XMFLOAT3(-1, 0, 0);
	XMFLOAT3 right = XMFLOAT3(1, 0, 0);
	XMFLOAT3 front = XMFLOAT3(0, 0, -1);
	XMFLOAT3 back = XMFLOAT3(0, 0, 1);
	//up0.x *= -1;
	//up0.y *= -1;
	//up0.z *= -1;
	XMFLOAT2 tx00 = XMFLOAT2(0, 0);
	XMFLOAT2 tx01 = XMFLOAT2(0, 1);
	XMFLOAT2 tx10 = XMFLOAT2(1, 0);
	XMFLOAT2 tx11 = XMFLOAT2(1, 1);

	// Assign vertex positions to correct vertices
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
	/*
	// bottom
	normal = down;
	vertices[i].position = vertex001;
	vertices[i].normal = normal;
	vertices[i].texture = tx00;
	indices[i] = i;
	i++;
	vertices[i].position = vertex100;
	vertices[i].normal = normal;
	vertices[i].texture = tx11;
	indices[i] = i;
	i++;
	vertices[i].position = vertex000;
	vertices[i].normal = normal;
	vertices[i].texture = tx10;
	indices[i] = i;
	i++;
	vertices[i].position = vertex001;
	vertices[i].normal = normal;
	vertices[i].texture = tx00;
	indices[i] = i;
	i++;
	vertices[i].position = vertex101;
	vertices[i].normal = normal;
	vertices[i].texture = tx10;
	indices[i] = i;
	i++;
	vertices[i].position = vertex100;
	vertices[i].normal = normal;
	vertices[i].texture = tx11;
	indices[i] = i;
	//i++;*/

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

/*
void ProcCubeMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Calculate vertex count
	// 6 vertices per quad, times 6 for each face
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6 * 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	// Vertex variables
	float yincrement = 2.0f;
	float xincrement = 2.0f;
	float ystart = 1.0f;
	float xstart = -1.0f;
	//UV variables
	float txu = 0.0f;
	float txv = 0.0f;
	float txuinc = 1.0f;	// UV increment
	float txvinc = 1.0f;
	//Counters
	int v = 0;	// vertex counter
	int i = 0;	// index counter

				//front face
	int m_resolution = 1;
	for (int y = 0; y<m_resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < m_resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].position = XMFLOAT3(xstart, ystart - yincrement, -1.0f);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(xstart + xincrement, ystart, -1.0f);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart, ystart, -1.0f);  // Top left.	-1.0, 1.0
			vertices[v].texture = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].position = XMFLOAT3(xstart, ystart - yincrement, -1.0f);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].position = XMFLOAT3(xstart + xincrement, ystart - yincrement, -1.0f);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(xstart + xincrement, ystart, -1.0f);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart += xincrement;
			txu += txuinc;
			//ystart -= yincrement;

		}

		ystart -= yincrement;
		xstart = -1;

		txu = 0;
		txv += txvinc;

	}

	txv = 0;

	//back face
	ystart = 1;
	xstart = 1;
	for (int y = 0; y<m_resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < m_resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].position = XMFLOAT3(xstart, ystart - yincrement, 1.0f);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart - xincrement, ystart, 1.0f);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(xstart, ystart, 1.0f);  // Top left.	-1.0, 1.0
			vertices[v].texture = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].position = XMFLOAT3(xstart, ystart - yincrement, 1.0f);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].position = XMFLOAT3(xstart - xincrement, ystart - yincrement, 1.0f);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart - xincrement, ystart, 1.0f);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart -= xincrement;
			//ystart -= yincrement;
			txu += txuinc;

		}

		ystart -= yincrement;
		xstart = 1;

		txu = 0;
		txv += txvinc;

	}

	txv = 0;

	//right face
	ystart = 1;
	xstart = -1;
	for (int y = 0; y<m_resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < m_resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].position = XMFLOAT3(1.0f, ystart - yincrement, xstart);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(1.0f, ystart, xstart + xincrement);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(1.0f, ystart, xstart);  // Top left.	-1.0, 1.0
			vertices[v].texture = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].position = XMFLOAT3(1.0f, ystart - yincrement, xstart);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].position = XMFLOAT3(1.0f, ystart - yincrement, xstart + xincrement);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(1.0f, ystart, xstart + xincrement);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart += xincrement;
			//ystart -= yincrement;
			txu += txuinc;

		}

		ystart -= yincrement;
		xstart = -1;
		txu = 0;
		txv += txvinc;
	}

	txv = 0;

	//left face
	ystart = 1;
	xstart = 1;
	for (int y = 0; y<m_resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < m_resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].position = XMFLOAT3(-1.0f, ystart - yincrement, xstart);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(-1.0f, ystart, xstart - xincrement);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(-1.0f, ystart, xstart);  // Top left.	-1.0, 1.0
			vertices[v].texture = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].position = XMFLOAT3(-1.0f, ystart - yincrement, xstart);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].position = XMFLOAT3(-1.0f, ystart - yincrement, xstart - xincrement);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(-1.0f, ystart, xstart - xincrement);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart -= xincrement;
			//ystart -= yincrement;
			txu += txuinc;
		}

		ystart -= yincrement;
		xstart = 1;
		txu = 0;
		txv += txvinc;
	}

	txv = 0;

	//top face
	ystart = 1;
	xstart = -1;

	for (int y = 0; y<m_resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < m_resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].position = XMFLOAT3(xstart, 1.0f, ystart - yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart + xincrement, 1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(xstart, 1.0f, ystart);  // Top left.	-1.0, 1.0
			vertices[v].texture = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].position = XMFLOAT3(xstart, 1.0f, ystart - yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].position = XMFLOAT3(xstart + xincrement, 1.0f, ystart - yincrement);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart + xincrement, 1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart += xincrement;
			//ystart -= yincrement;
			txu += txuinc;
		}

		ystart -= yincrement;
		xstart = -1;
		txu = 0;
		txv += txvinc;
	}

	txv = 0;

	//bottom face
	ystart = -1;
	xstart = -1;

	for (int y = 0; y<m_resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < m_resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].position = XMFLOAT3(xstart, -1.0f, ystart + yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart + xincrement, -1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].position = XMFLOAT3(xstart, -1.0f, ystart);  // Top left.	-1.0, 1.0
			vertices[v].texture = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].position = XMFLOAT3(xstart, -1.0f, ystart + yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texture = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].position = XMFLOAT3(xstart + xincrement, -1.0f, ystart + yincrement);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].position = XMFLOAT3(xstart + xincrement, -1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texture = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart += xincrement;
			//ystart -= yincrement;
			txu += txuinc;
		}

		ystart += yincrement;
		xstart = -1;
		txu = 0;
		txv += txvinc;
	}


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
*/

float ProcCubeMesh::GetHeight(float xPos, float zPos)
{
	float noiseX = m_Noise->Resolution * xPos / xSegs + m_Noise->Location.x;
	float noiseZ = m_Noise->Resolution * zPos / zSegs + m_Noise->Location.y;
	return (float)perlin->noise(noiseX, 0.0f, noiseZ);
}

XMFLOAT3 ProcCubeMesh::Cross(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 c = XMFLOAT3();
	c.x = (a.y*b.z) - (a.z*b.y);
	c.y = (a.z*b.x) - (a.x*b.z);
	c.z = (a.x*b.y) - (a.y*b.x);
	return c;
}

XMFLOAT3 ProcCubeMesh::Subtract(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 c = XMFLOAT3();
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	XMStoreFloat3(&c, XMVector3Normalize(XMLoadFloat3(&c)));
	/*
	float cMag = sqrtf((c.x*c.x) + (c.y*c.y) + (c.z*c.z));
	c.x /= cMag;
	c.y /= cMag;
	c.z /= cMag;
	*/
	return c;
}



