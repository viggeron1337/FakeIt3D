#pragma once
#include <Windows.h>
#include "Extern.h"

class GameWindow
{
private:
	WNDCLASSEXW m_wcex; 
	HWND m_wndHandler; 
	HINSTANCE m_hInstance; 

	HRESULT _init();

	int _setWcex();
	int  _createWindow();
	HRESULT _connectDirect3D(); 

	int m_test; 


public:
	GameWindow();
	~GameWindow(); 

	int start(); 

	static LRESULT CALLBACK StaticWndProc(
		_In_ HWND   hwnd,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);
	LRESULT CALLBACK WndProc(
		_In_ HWND   hwnd,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);
};