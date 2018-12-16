#pragma once
#include <d3d11.h>
class ForwardRenderer
{
private:
	HRESULT createDepthBufferStencil(); 
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RenderTargetView* m_pRenderTargetView; 

	void createInputDescription(); 



public:
	ForwardRenderer(); 
	~ForwardRenderer(); 
};