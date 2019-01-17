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
		DirectX::XMFLOAT4X4A world; 
	};

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_constantBufferPtr; 
	HRESULT createMesh(); 

	DirectX::XMFLOAT4A m_position; 

	DirectX::XMMATRIX m_worldMatrix; 

	CONSTANT_BUFFER m_constantBufferData; 

	void createConstantBuffer(); 

public:
	Object(); 
	~Object(); 

	void setPosition(float x, float y, float z); 
	DirectX::XMFLOAT4A& getPosition(); 

	void Move(float x, float y, float z); 

	CONSTANT_BUFFER& getBufferData(); 

	ID3D11Buffer* getConstantBufferPtr(); 

	HRESULT Init();
	HRESULT tempInitZTriangle();

	void draw(); 

	ID3D11Buffer* getBufferPtr(); 
};