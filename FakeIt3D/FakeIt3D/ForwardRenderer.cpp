#include "ForwardRenderer.h"
#include "GameWindow.h"
#include <chrono>
#include <ctime>

HRESULT ForwardRenderer::createSwapChain(HWND* wndHandler)
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 640;
	sd.BufferDesc.Height = 480;
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

	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = 640;
	descDepth.Height = 480;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = DX::g_device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; 
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; 
	depthStencilViewDesc.Texture2D.MipSlice = 0; 

	hr = DX::g_device->CreateDepthStencilView(pDepthStencil, &depthStencilViewDesc, &m_pDepthStencilView);

	DX::g_deviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView); 

	return hr; 
}

HRESULT ForwardRenderer::createRenderTargetView()
{
	HRESULT  hr; 
	ID3D11Texture2D* backBufferTex; 
	
	//Fills the Texture2D pointer with the back buffer
	DX::g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTex);
	
	//Create Resource out of backBuffer
	hr = DX::g_device->CreateRenderTargetView(backBufferTex, nullptr, &m_pRenderTargetView); 
	
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
	return E_NOTIMPL;
}

void ForwardRenderer::createInputDescription()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT,0, 20,  
			D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, 
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//DX::g_device->CreateInputLayout(layout, 4,)
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


