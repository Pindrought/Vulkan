#include "WindowClass.h"
#include "CustomStructs.h"
#include <iostream>
#ifdef VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#endif

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

bool WindowClass::Initialize(int width, int height, std::string window_title, std::string classname)
{
	this->width = width;
	this->height = height;
	this->classname = classname;

	std::wstring tempwindowtitle = std::wstring(window_title.begin(), window_title.end());
	LPCWSTR windowtitlewide = tempwindowtitle.c_str();

	std::wstring tempclassname = std::wstring(classname.begin(), classname.end());
	LPCWSTR classnamewide = tempclassname.c_str();

#ifdef VK_USE_PLATFORM_WIN32_KHR
	window_parameters.hInstance = GetModuleHandle(nullptr);
	WNDCLASSEX window_class = 
	{
		sizeof(WNDCLASSEX),             // UINT         cbSize
		/* Win 3.x */
		CS_HREDRAW | CS_VREDRAW,          // UINT         style
		WindowProcedure,                  // WNDPROC      lpfnWndProc
		0,                                // int          cbClsExtra
		0,                                // int          cbWndExtra
		window_parameters.hInstance,       // HINSTANCE    hInstance
		nullptr,                          // HICON        hIcon
		LoadCursor(nullptr, IDC_ARROW), // HCURSOR      hCursor
		(HBRUSH)(COLOR_WINDOW + 1),       // HBRUSH       hbrBackground
		nullptr,                          // LPCSTR       lpszMenuName
		classnamewide,         // LPCSTR       lpszClassName
					/* Win 4.0 */
		nullptr                           // HICON        hIconSm
	};

	if (!RegisterClassEx(&window_class))
	{
		std::cerr << "Failed to register window class." << std::endl;
		return false;
	}

	
	window_parameters.hWnd = CreateWindow(classnamewide, windowtitlewide, WS_OVERLAPPEDWINDOW, 100, 100, width, height, nullptr, nullptr, window_parameters.hInstance, nullptr);
	if (!window_parameters.hWnd)
	{
		std::cerr << "Failed to Create Window." << std::endl;
		return false;
	}
#elif defined VK_USE_PLATFORM_XLIB_KHR
	#error("Not yet implemented.")
#elif defined VK_USE_PLATFORM_XCB_KBR
	#error("Not yet implemented.")
#endif
	initialized = true;
	return true;
}

bool WindowClass::Show()
{
	return ShowWindow(window_parameters.hWnd, 1);
}

bool WindowClass::Hide()
{
	return ShowWindow(window_parameters.hWnd, 0);
}

void WindowClass::Release()
{
	if (window_parameters.hWnd != NULL)
	{
		Hide();
		auto ret = DestroyWindow(window_parameters.hWnd);
		if (ret == 0)
		{
			std::cerr << "Window destroying failed. GetLastError = " << GetLastError() << std::endl;
		}
		window_parameters.hWnd = NULL;
	}

	std::wstring tempclassname = std::wstring(classname.begin(), classname.end());
	LPCWSTR classnamewide = tempclassname.c_str();
	auto ret = UnregisterClass(classnamewide, window_parameters.hInstance);
	if (ret == 0)
	{
		std::cerr << "Window class unregistration failed. GetLastError = " << GetLastError() << std::endl;
	}
}
