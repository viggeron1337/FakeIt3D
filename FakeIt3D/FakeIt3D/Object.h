#pragma once 
#include <DirectXMath.h>
#include <d3d11.h>


//Vertex for the 2D Objects
struct Vertex
{
	DirectX::XMFLOAT4A Pos; 
	DirectX::XMFLOAT4A Color;
};

class Object
{
private:

	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4A m_worldMatrix; 
	};

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	HRESULT createMesh(); 

public:
	Object(); 
	~Object(); 


	HRESULT Init();
	HRESULT tempInitZTriangle();

	void draw(); 

	ID3D11Buffer* getBufferPtr(); 
};