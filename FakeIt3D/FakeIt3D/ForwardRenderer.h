#pragma once
#include <d3d11.h>
class ForwardRenderer
{
private:
	
	D3D11_VIEWPORT m_viewPort; 

	HRESULT createSwapChain(HWND* wndHandler); 
	HRESULT createRenderTargetView(); 
	HRESULT createZBuffer(); 


	//Render Target, a texture to render to. 
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr; 

	//To be used to fill with data of the back buffer when neccesary. 
	D3D11_TEXTURE2D_DESC m_backBufferDesc; 

	//Z-Buffer 
	ID3D11Texture2D* m_pZbufferTex;
	ID3D11DepthStencilState* m_pZbufferState; 
	ID3D11DepthStencilView* m_pZbufferView; 

public:

	HRESULT init(HWND* wndHandler); 

	void beginFrame(); 
	void endFrame(); 

	ForwardRenderer(); 
	~ForwardRenderer(); 
};