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
		DirectX::XMFLOAT4A Color;
	};


	HRESULT createMesh(); 
	HRESULT createShaders(); 

	//To be changed to shared shaders / input layouts for all objects of the same type, just to get things moving
	ID3D11VertexShader* m_pVertexShader = nullptr; 
	ID3D11PixelShader* m_pPixelShader = nullptr; 
	ID3D11InputLayout* m_pInputLayout = nullptr; 

public:
	Object(); 
	~Object(); 

	HRESULT Init();

	HRESULT tempInitZTriangle();

	void draw(); 

	ID3D11Buffer* getBufferPtr(); 

};