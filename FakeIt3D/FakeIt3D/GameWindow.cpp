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

	HRESULT hr;

	//LPCTSTR szTitle = "FakeIt3D"; 

	m_wndHandler = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		m_wcex.lpszClassName,
		"This is a Unicode string",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480,
		NULL,
		NULL,
		m_wcex.hInstance,
		NULL
	);

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

LRESULT GameWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK procProxy(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Get the window pointer we stored in the extra bits of the window instance. 
	GameWindow* pWindow = (GameWindow*)GetWindowLongPtrW(handle, GWLP_USERDATA);

	//Use the collected pointer to call the member version of the WndProc. We can now use
	//member variables in our WndProc function, convinient. 
	if (pWindow->WndProc(handle,message, wParam, lParam))
	{
		return 0;
	}

	return DefWindowProcW(handle, message, wParam, lParam);
}

LRESULT GameWindow::StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if (uMsg  == WM_CREATE)
	{

		//When you receive the WM_CREATE message, the lParam parameter of each message is 
		//a pointer to a CREATESTRUCT structure. The CREATESTRUCT structure, in turn, contains the pointer that you passed into CreateWindowEx.
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;

		//Store the data from the parameters into the free space in the instance of the window (A pointer to our window class). 
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);

		//Here, the program is told that whenever our WndProc is called from now on, execute the procProxy function. 
		SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)procProxy);

		return 0;
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}


