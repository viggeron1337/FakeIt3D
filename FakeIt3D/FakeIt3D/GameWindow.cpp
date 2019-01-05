#include "GameWindow.h"
#include <tchar.h>
#include "Object.h"
#include <iostream>

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
	m_wcex.lpszClassName = L"GameWindow"; 
	m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassExW(&m_wcex))
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

	LPCWSTR szTitle = L"FakeIt3D"; 

	m_wndHandler = CreateWindowW(
		m_wcex.lpszClassName,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
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
	//Create forward renderer for window
	hr = m_frwdRenderer.init(&m_wndHandler); 
	return hr;
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
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

GameWindow::GameWindow()
{
	HRESULT hr;
	hr = _init(); 

	if (hr != S_OK)
	{
		MessageBox(NULL,
			_T("Direct3D initialization failed!"),
			_T("FakeIt3D"),
			NULL);

		exit(0); 
	}
}

GameWindow::~GameWindow()
{
}

int GameWindow::start()
{
	//Test Objects
	Object triangle;
	Object triangleZ; 
	triangle.Init();
	triangleZ.tempInitZTriangle(); 


	//Handle messages to the window 
	MSG msg; 
	msg.message = WM_NULL; 

	//Init loop variables. 
	m_frames = 0, m_ticks = 0; 
	m_time = steady_clock::now(); 
	m_unprocessed = 0; 
	m_timer = steady_clock::now();
	m_frequency = 1000000000.0f / 60.0f; 

	while (WM_QUIT != msg.message)
	{
		//Get current time. 
		steady_clock::time_point now = steady_clock::now(); 

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		//Get the deltatime since last frame. 
		auto dt = duration_cast<nanoseconds>(now - m_time).count(); 
		
		//Update the last time. 
		m_time = steady_clock::now(); 
		
		m_unprocessed += dt / m_frequency; 


		if (m_unprocessed > 1)
		{
			m_ticks++; 
			m_unprocessed -= 1; 

			//Update everything. 
		}
		m_frames++; 


		m_frwdRenderer.beginFrame(); 
		
		//Submit to draw queues (temp)
		triangle.draw(); 
		triangleZ.draw(); 
		
		//Render
		m_frwdRenderer.Flush(); 

		m_frwdRenderer.endFrame(); 

		if (duration_cast<milliseconds>(steady_clock::now() - m_timer).count() > 1000)
		{
			std::cout << "FPS: " << m_frames << "TICK: " << m_ticks << std::endl; 
			m_ticks = 0; 
			m_frames = 0; 
			m_timer += milliseconds(1000); 
		}
	}

	return (int)msg.wParam;
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


