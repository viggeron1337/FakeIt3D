#include "Object.h"
#include <d3d11.h>
#include "Extern.h"

Object::Object()
{
}

Object::~Object()
{
}

ID3D11Buffer * Object::getBufferPtr()
{
	return m_pVertexBuffer; 
}

HRESULT Object::Init()
{
	HRESULT hr;
	//Create vertexBuffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 3;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_vertexBufferData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	hr = DX::g_device->CreateBuffer(&bufferDesc, &InitData, &m_pVertexBuffer);



	return hr; 
}
