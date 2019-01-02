#pragma once
#include <d3d11.h>
class ForwardRenderer
{
private:
	
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11DepthStencilState* m_pDepthStencilState; 
	ID3D11Texture2D* m_pTexture;
	ID3D11SamplerState* m_pSamplerState; 
	D3D11_VIEWPORT m_viewPort; 

	HRESULT createSwapChain(HWND* wndHandler); 
	HRESULT createRenderTargetView(); 
	HRESULT createDepthBufferStencil(); 
	HRESULT createDepthStencilState(); 
	HRESULT createSamplerState(); 
	HRESULT createViewPort(); 

	void createInputDescription();

	//Render Target, a texture to render to. 
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr; 

public:

	HRESULT init(HWND* wndHandler); 

	void beginFrame(); 
	void endFrame(); 

	ForwardRenderer(); 
	~ForwardRenderer(); 
};