#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
namespace DX
{
	extern ID3D11Device* g_device; 
	extern ID3D11DeviceContext* g_deviceContext; 
	extern IDXGISwapChain* g_swapChain; 
}