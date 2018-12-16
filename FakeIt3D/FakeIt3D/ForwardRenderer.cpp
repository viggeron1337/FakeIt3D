#include "ForwardRenderer.h"
#include "GameWindow.h"

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

	//DX::g_device->CreateInputLayout(layout, 4, VS->)
}

ForwardRenderer::ForwardRenderer()
{
}

ForwardRenderer::~ForwardRenderer()
{
}
