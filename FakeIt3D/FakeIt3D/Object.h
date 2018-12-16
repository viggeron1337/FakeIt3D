#pragma once 
#include <DirectXMath.h>
#include <d3d11.h>

//This is just a temporary class for testing rendering. 

class Object
{
private:

	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4A m_worldMatrix; 
	};

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	struct Vertex
	{
		DirectX::XMFLOAT4A Pos; 
		DirectX::XMFLOAT2A UV; 
		DirectX::XMFLOAT4A Color;
		DirectX::XMFLOAT4A Normal; 
	};

	Vertex m_vertexBufferData[3] =
	{
		DirectX::XMFLOAT4A(0.0f,0.0f,0.0f,1.0f),
		DirectX::XMFLOAT2A(0.0f,0.0f),
		DirectX::XMFLOAT4A(1.0f,0.0f,0.0f,0.0f),
		DirectX::XMFLOAT4A(0.0f,0.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT4A(0.0f,1.0f,0.0f, 1.0f),
		DirectX::XMFLOAT2A(0.0f,1.0f),
		DirectX::XMFLOAT4A(0.0f,1.0f,0.0f,0.0f),
		DirectX::XMFLOAT4A(0.0f,0.0f,-1.0f,0.0f),
		DirectX::XMFLOAT4A(1.0f,0.0f,0.0f,1.0f),
		DirectX::XMFLOAT2A(1.0f,0.0f),
		DirectX::XMFLOAT4A(0.0f,0.0f,1.0f,0.0f),
		DirectX::XMFLOAT4A(0.0f,0.0f,-1.0f,0.0f),
	};

public:
	Object(); 
	~Object(); 

	HRESULT Init(); 

	ID3D11Buffer* getBufferPtr(); 

};