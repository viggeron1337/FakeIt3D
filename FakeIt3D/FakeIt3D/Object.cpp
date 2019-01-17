#include "Object.h"
#include <d3d11.h>
#include "Extern.h"
#include "ExternRenderQueue.h"
#include <fstream>
#include <vector>

HRESULT Object::createMesh()
{

	Vertex m_vertexBufferData[3] =
	{
		//Defining vetices of a triangle
		DirectX::XMFLOAT4A(-1.0f,-1.0f,0.0f, 0.0f),
		DirectX::XMFLOAT4A(1.0, 0.0, 0.0, 1.0f),
		DirectX::XMFLOAT4A(0.0f,1.0f,0.0f, 0.0f),
		DirectX::XMFLOAT4A(0.0, 1.0, 0.0, 1.0f),
		DirectX::XMFLOAT4A(1.0f,-1.0f,0.0f, 0.0f),
		DirectX::XMFLOAT4A(0.0, 0.0, 1.0, 1.0f),
	};

	HRESULT hr;
	//Create vertexBuffer which can be sent to and processed by the GPU. 
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 3;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data (The vertex data to send to the GPU).
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_vertexBufferData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	hr = DX::g_device->CreateBuffer(&bufferDesc, &InitData, &m_pVertexBuffer);

	createConstantBuffer(); 

	return hr; 
}

void Object::createConstantBuffer()
{
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;
	buffDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA pBufferData;
	pBufferData.pSysMem = &m_constantBufferData;
	pBufferData.SysMemPitch = 0;
	pBufferData.SysMemSlicePitch = 0;

	DX::g_device->CreateBuffer(&buffDesc, &pBufferData, &m_constantBufferPtr);
}

Object::Object()
{
}

Object::~Object()
{
	m_pVertexBuffer->Release(); 
}

void Object::setPosition(float x, float y, float z)
{
	
}

DirectX::XMFLOAT4A & Object::getPosition()
{
	return m_position; 
}

void Object::Move(float x, float y, float z)
{
	m_worldMatrix = DirectX::XMMatrixTranslation(x, y, z); 

	//Make sure to save the movement in the buffer. 
	DirectX::XMStoreFloat4x4A(&m_constantBufferData.world, XMMatrixTranspose(m_worldMatrix));
}

Object::CONSTANT_BUFFER & Object::getBufferData()
{
	return m_constantBufferData; 
}

ID3D11Buffer * Object::getConstantBufferPtr()
{
	return m_constantBufferPtr; 
}

ID3D11Buffer * Object::getBufferPtr()
{
	return m_pVertexBuffer; 
}

HRESULT Object::Init()
{
	HRESULT hr; 

	hr = createMesh();

	return hr; 
}

HRESULT Object::tempInitZTriangle()
{
	Vertex m_vertexBufferData[3] =
	{
		//Defining vetices of a triangle
		DirectX::XMFLOAT4A(-1.0f,-1.0f,1.0f, 0.0f),
		DirectX::XMFLOAT4A(1.0, 0.0, 0.0, 1.0f),
		DirectX::XMFLOAT4A(0.0f,1.0f,1.0f, 0.0f),
		DirectX::XMFLOAT4A(1.0, 0.0, 0.0, 1.0f),
		DirectX::XMFLOAT4A(1.0f,-1.0f,1.0f, 0.0f),
		DirectX::XMFLOAT4A(1.0, 0.0, 0.0, 1.0f),
	};

	HRESULT hr;
	//Create vertexBuffer which can be sent to and processed by the GPU. 
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 3;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data (The vertex data to send to the GPU).
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_vertexBufferData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	hr = DX::g_device->CreateBuffer(&bufferDesc, &InitData, &m_pVertexBuffer);

	//Init translation matrix. 
	m_worldMatrix = DirectX::XMMatrixTranslation(0.f, 0.f, 0.f);

	return hr; 
}

void Object::draw()
{
	//Submit to 2D draw queue.
	RenderQueue::g_Q2DObjects.push_back(this); 
}
