// Mesh.cpp
#include "mesh.h"
#include "ProcMesh.h"

ProcMesh::ProcMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int cellX, int cellZ, int xSegments, int zSegments)
{
	bool result;
	x = cellX;
	z = cellZ;
	xSegs = xSegments;
	zSegs = zSegments;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitBuffers(device);
	if (!result)
	{
		MessageBox(NULL, L"Mesh buffer failure", L"ERROR", MB_OK);
		exit(0);
	}

	// Load the texture for this model.
	LoadTexture(device, deviceContext, textureFilename);

}


ProcMesh::~ProcMesh()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	// Release the texture object.
	if (m_Texture)
	{
		delete m_Texture;
		m_Texture = 0;
	}
}


int ProcMesh::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ProcMesh::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ProcMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	// Set the gap between cells (for gridlines)
	float border = 0.0f;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 8;

	// Set the number of indices in the index array.
	m_indexCount = 36;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	float height00 = GetHeight(x - border, z - border);
	float height01 = GetHeight(x - border, z + border);
	float height10 = GetHeight(x + border, z - border);
	float height11 = GetHeight(x + border, z + border);

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3((1 - border), height00 * m_Noise->Height, (1 - border));  // vertex100.
	vertices[0].texture = XMFLOAT2(1.0f, 0.0f);
	XMVECTOR vertex100 = XMLoadFloat3(&vertices[0].position);
	//vertices[0].normal = /*XMFLOAT3(0.0f, 0.0f, -1.0f);*/ 

	vertices[1].position = XMFLOAT3((1 - border), height01 * m_Noise->Height, (border - 1));  // vertex101.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);
	XMVECTOR vertex101 = XMLoadFloat3(&vertices[1].position);

	vertices[2].position = XMFLOAT3((border - 1), height10 * m_Noise->Height, (1 - border));  // vertex110.
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	XMVECTOR vertex110 = XMLoadFloat3(&vertices[2].position);

	vertices[3].position = XMFLOAT3((border - 1), height11 * m_Noise->Height, (border - 1));  // vertex111.
	vertices[3].texture = XMFLOAT2(1.0f, 1.0f);
	XMVECTOR vertex111 = XMLoadFloat3(&vertices[3].position);

	vertices[4].position = XMFLOAT3((1 - border), -1.0f, (1 - border));  // vertex000.
	vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
	XMVECTOR vertex000 = XMLoadFloat3(&vertices[4].position);

	vertices[5].position = XMFLOAT3((1 - border), -1.0f, (border - 1));  // vertex001.
	vertices[5].texture = XMFLOAT2(0.0f, 1.0f);
	XMVECTOR vertex001 = XMLoadFloat3(&vertices[5].position);

	vertices[6].position = XMFLOAT3((border - 1), -1.0f, (1 - border));  // vertex010.
	vertices[6].texture = XMFLOAT2(0.0f, 0.0f);
	XMVECTOR vertex010 = XMLoadFloat3(&vertices[6].position);

	vertices[7].position = XMFLOAT3((border - 1), -1.0f, (border - 1));  // vertex011.
	vertices[7].texture = XMFLOAT2(0.0f, 1.0f);
	XMVECTOR vertex011 = XMLoadFloat3(&vertices[7].position);

	XMVECTOR normal000111 = XMVector3Cross(vertex101 - vertex100, vertex111 - vertex100);
	XMVECTOR normal001011 = XMVector3Cross(vertex111 - vertex100, vertex110 - vertex100);
	XMStoreFloat3(&vertices[0].normal, XMVector3Normalize(normal000111));
	
	

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 2;  // Top right.
	indices[2] = 1;  // Top left.

	indices[3] = 0;	// bottom left
	indices[4] = 3;	// bottom right
	indices[5] = 2;	// top right

	//
	// Set up descriptions and clean up
	//				
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
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

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
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ProcMesh::SendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ProcMesh::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	// Create the texture object.
	m_Texture = new Texture(device, deviceContext, filename);
}

float ProcMesh::GetHeight(float xPos, float zPos)
{
	float noiseX = m_Noise->Resolution * xPos / xSegs + m_Noise->Location.x;
	float noiseZ = m_Noise->Resolution * zPos / zSegs + m_Noise->Location.y;
	//return Mathf.PerlinNoise(noiseX, noiseY);
	return 1.0f;
}
