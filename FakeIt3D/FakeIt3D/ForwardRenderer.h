#pragma once
#include <d3d11.h>
#include "Object.h"
#include <vector>

class ForwardRenderer
{
private:
	
	D3D11_VIEWPORT m_viewPort; 

	HRESULT _createSwapChain(HWND* wndHandler); 
	HRESULT _createRenderTargetView(); 
	HRESULT _createZBuffer(); 


	//Render Target, a texture to render to. 
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr; 

	//To be used to fill with data of the back buffer when neccesary. 
	D3D11_TEXTURE2D_DESC m_backBufferDesc; 

	//Z-Buffer 
	ID3D11Texture2D* m_pZbufferTex;
	ID3D11DepthStencilState* m_pZbufferState; 
	ID3D11DepthStencilView* m_pZbufferView; 

	void _setup2DPass();
	void _setup3DPass(); 

	HRESULT _initLayoutsAndShaders();

public:

	HRESULT init(HWND* wndHandler); 

	void beginFrame(); 
	void endFrame(); 

	void pass2D(); 

	ForwardRenderer(); 
	~ForwardRenderer(); 
};