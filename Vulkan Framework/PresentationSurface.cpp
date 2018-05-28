#include "PresentationSurface.h"
#include "VulkanFunctions.h"

bool PresentationSurface::Initialize(VkInstance vulkan_instance, WindowParameters window_parameters)
{
	this->vulkan_instance = vulkan_instance;

	VkResult result;

#ifdef VK_USE_PLATFORM_WIN32_KHR

	VkWin32SurfaceCreateInfoKHR surface_create_info =
	{
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                 sType
		nullptr,                                          // const void                    * pNext
		0,                                                // VkWin32SurfaceCreateFlagsKHR    flags
		window_parameters.hInstance,                      // HINSTANCE                       hinstance
		window_parameters.hWnd                            // HWND                            hwnd
	};

	result = PVF::vkCreateWin32SurfaceKHR(vulkan_instance, &surface_create_info, nullptr, &handle);

#elif defined VK_USE_PLATFORM_XLIB_KHR

	VkXlibSurfaceCreateInfoKHR surface_create_info =
	{
		VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,   // VkStructureType                 sType
		nullptr,                                          // const void                    * pNext
		0,                                                // VkXlibSurfaceCreateFlagsKHR     flags
		window_parameters.Dpy,                            // Display                       * dpy
		window_parameters.Window                          // Window                          window
	};

	result = vkCreateXlibSurfaceKHR(instance, &surface_create_info, nullptr, &presentation_surface);

#elif defined VK_USE_PLATFORM_XCB_KHR

	VkXcbSurfaceCreateInfoKHR surface_create_info =
	{
		VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,    // VkStructureType                 sType
		nullptr,                                          // const void                    * pNext
		0,                                                // VkXcbSurfaceCreateFlagsKHR      flags
		window_parameters.Connection,                     // xcb_connection_t              * connection
		window_parameters.Window                          // xcb_window_t                    window
	};

	result = vkCreateXcbSurfaceKHR(instance, &surface_create_info, nullptr, &presentation_surface);

#endif

	if ((VK_SUCCESS != result) || (handle == VK_NULL_HANDLE))
	{
		std::cerr << "Could not create presentation surface." << std::endl;
		return false;
	}
	return true;
}

void PresentationSurface::Release()
{
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroySurfaceKHR(vulkan_instance, handle, nullptr);
		initialized = false;
		handle = VK_NULL_HANDLE;
	}
}
