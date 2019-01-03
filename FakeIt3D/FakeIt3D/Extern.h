#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
namespace DX
{
	//D3D
	extern ID3D11Device* g_device; 
	extern ID3D11DeviceContext* g_deviceContext; 
	extern IDXGISwapChain* g_swapChain;

	//Shaders
	extern ID3D11VertexShader* g_pVertexShader; 
	extern ID3D11PixelShader* g_pPixelShader; 

	//Layouts 
	extern ID3D11InputLayout* g_pInputLayout; 
}