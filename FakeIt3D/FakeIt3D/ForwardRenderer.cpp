#include "ForwardRenderer.h"
#include "GameWindow.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include "ExternRenderQueue.h"
#include "Extern.h"

//Initialize the extern queues
std::vector<Object*> RenderQueue::g_Q2DObjects;

//Initialze Shaders and Layout

ID3D11VertexShader* DX::g_pVertexShader = nullptr;
ID3D11PixelShader* DX::g_pPixelShader = nullptr;
ID3D11InputLayout* DX::g_pInputLayout = nullptr;


HRESULT ForwardRenderer::_createSwapChain(HWND* wndHandler)
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


HRESULT ForwardRenderer::_createRenderTargetView()
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

HRESULT ForwardRenderer::_createZBuffer()
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


void ForwardRenderer::_setup2DPass()
{
	//Set the shaders
	DX::g_deviceContext->VSSetShader(DX::g_pVertexShader, nullptr, 0);
	DX::g_deviceContext->PSSetShader(DX::g_pPixelShader, nullptr, 0);

	//Set the layout
	DX::g_deviceContext->IASetInputLayout(DX::g_pInputLayout); 
}

void ForwardRenderer::_setup3DPass()
{
}

HRESULT ForwardRenderer::_initLayoutsAndShaders()
{
	HRESULT hr; 

	//////////////////////////////////2D/////////////////////////////////////////////////////////////////////////////

	//.cso is the sompiled versions of the shaders that are put in specified directory. 
	//Makes sure to specify that the .cso files are binary and opens them. 
	std::ifstream vsFile("Vertex.cso", std::ios::binary);
	std::ifstream psFile("Pixel.cso", std::ios::binary);


	//Creates an iterator that goes through the specified file (byte by byte as specified by the 'char' type;
	//Then adds the data byte by byte into the vector. (This is pretty slow though)
	std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	hr = DX::g_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &DX::g_pVertexShader);
	DX::g_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &DX::g_pPixelShader);

	// Create input layout so that proper communication between buffers and shaders can be done. 
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DX::g_device->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &DX::g_pInputLayout);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return hr;
}

HRESULT ForwardRenderer::init(HWND* wndHandler)
{
	HRESULT hr; 

	//Initialize forward renderer 
	hr = _createSwapChain(wndHandler); 
	_createRenderTargetView(); 
	hr = _createZBuffer(); 

	_initLayoutsAndShaders();

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

void ForwardRenderer::pass2D()
{

	_setup2DPass(); 
	
	//Set the stride for the 2D vertices. 
	UINT stride = sizeof(Vertex); 
	UINT offset = 0; 

	//Set vertex buffer and draw.
	for (int i = 0; i < RenderQueue::g_Q2DObjects.size(); i++)
	{
		ID3D11Buffer* vertexBuffer = RenderQueue::g_Q2DObjects[i]->getBufferPtr();
		DX::g_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride ,&offset); 		
		DX::g_deviceContext->Draw(3,0); 
	}


}

ForwardRenderer::ForwardRenderer()
{
}

ForwardRenderer::~ForwardRenderer()
{
	DX::g_pVertexShader->Release(); 
	DX::g_pPixelShader->Release(); 
	DX::g_pInputLayout->Release(); 
	DX::g_swapChain->Release(); 
	DX::g_device->Release(); 
	DX::g_deviceContext->Release(); 
}


