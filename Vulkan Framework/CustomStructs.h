#ifndef CUSTOMSTRUCTS_H
#define CUSTOMSTRUCTS_H
#include <vulkan\vulkan.h>
#include <vector>

namespace PVF
{
	struct QueueInfo
	{
		uint32_t           FamilyIndex;
		std::vector<float> Priorities;
	};

	struct QueueParameters 
	{
		VkQueue   Handle;
		uint32_t  FamilyIndex;
	};

	struct WaitSemaphoreInfo 
	{
		VkSemaphore           Semaphore;
		VkPipelineStageFlags  WaitingStage;
	};

	// OS-specific parameters
	struct WindowParameters {
#ifdef VK_USE_PLATFORM_WIN32_KHR

		HINSTANCE          HInstance;
		HWND               HWnd;

#elif defined VK_USE_PLATFORM_XLIB_KHR

		Display          * Dpy;
		Window             Window;

#elif defined VK_USE_PLATFORM_XCB_KHR

		xcb_connection_t * Connection;
		xcb_window_t       Window;

#endif
	};
}

#endif