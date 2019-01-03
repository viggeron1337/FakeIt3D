#include "ForwardRenderer.h"
#include "GameWindow.h"
#include <chrono>
#include <ctime>

HRESULT ForwardRenderer::createSwapChain(HWND* wndHandler)
{
	//Note: Did not specify resolution, so the resolution for the window 
	//held by wndHandler is used. 
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = *wndHandler;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&FeatureLevelsRequested,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&DX::g_swapChain,
		&DX::g_device,
		&FeatureLevelsSupported,
		&DX::g_deviceContext)))
	{
		return hr;
	}
}

HRESULT ForwardRenderer::createDepthBufferStencil()
{
	HRESULT hr; 

	//S_OK :)
	hr = S_OK; 

	return hr; 
}

HRESULT ForwardRenderer::createRenderTargetView()
{
	HRESULT  hr; 
	ID3D11Texture2D* backBufferTex; 
	
	//Fills the Texture2D pointer with the back buffer
	DX::g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTex);
	
	//Create View out of backBuffer Resource
	hr = DX::g_device->CreateRenderTargetView(backBufferTex, nullptr, &m_pRenderTargetView);
	
	//Fill the back buffer description with neccesary information before releasing
	backBufferTex->GetDesc(&m_backBufferDesc); 

	//We do not need this anymore lol
	backBufferTex->Release(); 

	return hr; 
}

HRESULT ForwardRenderer::createDepthStencilState()
{
	return E_NOTIMPL;
}

HRESULT ForwardRenderer::createSamplerState()
{
	return E_NOTIMPL;
}

HRESULT ForwardRenderer::createViewPort()
{
	HRESULT hr;

	//It's always OK :)
	hr = S_OK; 

	return hr; 
}

HRESULT ForwardRenderer::init(HWND* wndHandler)
{
	HRESULT hr; 

	//Initialize forward renderer 
	hr = createSwapChain(wndHandler); 
	createRenderTargetView(); 

	return hr; 
}

void ForwardRenderer::beginFrame()
{
	//Bind render target: OM stands for Output Merger
	DX::g_deviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr); 

	//set viewPort, this function constructs one. 
	//This is neccesay because the standard viewport is 0x0. 
	auto viewPort = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height); 

	//This is a setup for the razteriser, where the geometry is clipped against the viewport. 
	DX::g_deviceContext->RSSetViewports(1, &viewPort); 

	//Set the background color (Clear color) 
	float clearColor[] = { .25f, .5f, 1.0f, 1.0f };

	DX::g_deviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
}

void ForwardRenderer::endFrame()
{
	//Swaping the buffer, presenting the back buffer. 
	DX::g_swapChain->Present(1, 0); 
}

ForwardRenderer::ForwardRenderer()
{
}

ForwardRenderer::~ForwardRenderer()
{
}


