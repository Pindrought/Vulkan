#pragma once
#include <vulkan\vulkan.h>

struct WindowParameters 
{
#ifdef VK_USE_PLATFORM_WIN32_KHR

	HINSTANCE          hInstance = NULL;
	HWND               hWnd = NULL;

#elif defined VK_USE_PLATFORM_XLIB_KHR

	Display          * Dpy;
	Window             Window;

#elif defined VK_USE_PLATFORM_XCB_KHR

	xcb_connection_t * Connection;
	xcb_window_t       Window;

#endif

};