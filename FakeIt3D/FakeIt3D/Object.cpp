#include "Object.h"
#include <d3d11.h>
#include "Extern.h"
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

	return hr; 
}

HRESULT Object::createShaders()
{
	HRESULT hr; 

	//.cso is the sompiled versions of the shaders that are put in specified directory. 
	//Makes sure to specify that the .cso files are binary and opens them. 
	std::ifstream vsFile("Vertex.cso", std::ios::binary); 
	std::ifstream psFile("Pixel.cso", std::ios::binary);


	//Creates an iterator that goes through the specified file (byte by byte as specified by the 'char' type;
	//Then adds the data byte by byte into the vector. (This is pretty slow though)
	std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	
	hr = DX::g_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_pVertexShader); 
	DX::g_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pPixelShader); 

	// Create input layout so that proper communication between buffers and shaders can be done. 
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	}; 

	DX::g_device->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &m_pInputLayout); 

	return hr; 
}

Object::Object()
{
}

Object::~Object()
{
	m_pVertexBuffer->Release(); 
	m_pVertexShader->Release(); 
	m_pPixelShader->Release(); 
	m_pInputLayout->Release(); 
}

ID3D11Buffer * Object::getBufferPtr()
{
	return m_pVertexBuffer; 
}

HRESULT Object::Init()
{
	HRESULT hr; 

	hr = createMesh();
	createShaders(); 

	return hr; 
}

void Object::draw()
{
	//Bind vertex buffer for object

	//Memory offput between the vertices
	UINT stride = sizeof(Vertex);
	//Start to read here.
	UINT offset = 0;
	DX::g_deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Bind shaders and layout 
	DX::g_deviceContext->IASetInputLayout(m_pInputLayout); 
	DX::g_deviceContext->VSSetShader(m_pVertexShader, nullptr, 0); 
	DX::g_deviceContext->PSSetShader(m_pPixelShader, nullptr, 0); 

	//Draw
	DX::g_deviceContext->Draw(3, 0); 

}
