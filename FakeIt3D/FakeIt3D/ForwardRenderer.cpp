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

HRESULT ForwardRenderer::createZBuffer()
{
	HRESULT hr;  

	//Configure the texture to be used as a Z-Buffer

	D3D11_TEXTURE2D_DESC zTexDesc; 
	zTexDesc.Width = m_backBufferDesc.Width;
	zTexDesc.Height = m_backBufferDesc.Height;
	zTexDesc.MipLevels = 1; 
	zTexDesc.ArraySize = 1; 
	zTexDesc.Format = DXGI_FORMAT_D32_FLOAT; 
	zTexDesc.SampleDesc.Count = 1; 
	zTexDesc.SampleDesc.Quality = 0; 
	zTexDesc.Usage = D3D11_USAGE_DEFAULT; 
	zTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; 
	zTexDesc.CPUAccessFlags = 0;
	zTexDesc.MiscFlags = 0; 
	
	hr = DX::g_device->CreateTexture2D(&zTexDesc, NULL, &m_pZbufferTex);

	//Configure the settings for how the OM is to handle Z-checks. 

	D3D11_DEPTH_STENCIL_DESC zStateDesc; 

	// Depth test parameters
	zStateDesc.DepthEnable = true;
	zStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	zStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	zStateDesc.StencilEnable = true;
	zStateDesc.StencilReadMask = 0xFF;
	zStateDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	zStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	zStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	zStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	zStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	zStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	zStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	zStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	zStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	hr = DX::g_device->CreateDepthStencilState(&zStateDesc, &m_pZbufferState); 

	//Now bind the depth-stencil state to the OM stage. 
	if (hr == S_OK)
	{
		DX::g_deviceContext->OMSetDepthStencilState(m_pZbufferState, 1); 
	}

	//Make the resource (Zbuffer texture) into a view and bind it as a rendertarget for the OM to use. 
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = zTexDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0; 

	//Create depth-stencil view. 
	hr = DX::g_device->CreateDepthStencilView(m_pZbufferTex, &descDSV, &m_pZbufferView);

	//Bind it to the OM and tell it to compare and sample our rendertargetview. (guess)
	DX::g_deviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pZbufferView); 
	
	return hr; 
}


HRESULT ForwardRenderer::init(HWND* wndHandler)
{
	HRESULT hr; 

	//Initialize forward renderer 
	hr = createSwapChain(wndHandler); 
	createRenderTargetView(); 
	hr = createZBuffer(); 

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


