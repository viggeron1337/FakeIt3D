#pragma once
#include <Windows.h>
#include "Extern.h"
#include "ForwardRenderer.h"
#include <ctime>
#include <chrono>

using namespace std::chrono;

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

	ForwardRenderer m_frwdRenderer; 

	int m_test; 

	//Main loop variables
	steady_clock::time_point m_time;
	steady_clock::time_point m_timer; 
	int m_ticks; 
	int m_frames;
	float m_frequency; 
	float m_unprocessed; 

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