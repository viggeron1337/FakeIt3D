#include "GameWindow.h"
#include <tchar.h>

ID3D11Device* DX::g_device; 
ID3D11DeviceContext* DX::g_deviceContext; 
IDXGISwapChain* DX::g_swapChain; 

#pragma comment(lib, "d3d11.lib")

int GameWindow::_setWcex()
{
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc = StaticWndProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = m_hInstance;
	m_wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	m_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wcex.lpszMenuName = NULL;
	m_wcex.lpszClassName = "GameWindow"; 
	m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&m_wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return 0; 
	}
	return 1; 
}

int GameWindow::_createWindow()
{
 
	static TCHAR szTitle[] = _T("FakeIt3D");

	m_wndHandler = CreateWindow(
		m_wcex.lpszClassName,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640,480,
		NULL,
		NULL,
		m_wcex.hInstance,
		NULL
	);

	this->embedWndPtr(); 

	if (!m_wndHandler)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return 0;
	}

	ShowWindow(m_wndHandler, SW_SHOW); 

	return 1; 
}

HRESULT GameWindow::_connectDirect3D()
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
	sd.OutputWindow = m_wndHandler;
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

LRESULT GameWindow::_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Private _WndProc, the action happens here. 
	switch (uMsg)
	{

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

GameWindow::GameWindow()
{
	HRESULT hr;
	hr = _init(); 
}

GameWindow::~GameWindow()
{
}

int GameWindow::start()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void GameWindow::embedWndPtr()
{
	//Embed pointer to this class in the windows extra space. 
	SetWindowLongPtr(m_wndHandler,0,(LONG)this); 
}

HRESULT GameWindow::_init()
{ 
	_setWcex();
	_createWindow(); 
	return _connectDirect3D(); 
}

LRESULT GameWindow::StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Create a pointer to a gameWindow, and embed it in the extra bytes in the window instance. 
	//This pointer will be used to call a memberfunction variant of WndProc, which implicitly 
	//will pass a this pointer as a parameter, hence we will reach all of the memeber variables in that function. 
	
	GameWindow* thisGameWndPtr = nullptr; 

	if (uMsg == WM_CREATE)
	{
		SetWindowLong(hwnd, GWLP_USERDATA, (LONG_PTR)thisGameWndPtr);
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		thisGameWndPtr = (GameWindow*)GetWindowLong(hwnd, GWLP_USERDATA);
		if(!thisGameWndPtr) return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
		
	return thisGameWndPtr->_WndProc(hwnd, uMsg, wParam, lParam);
}


