#pragma once
#include <vulkan\vulkan.h>
#include "WindowParameters.h"

class PresentationSurface
{
public:
	VkSurfaceKHR handle = VK_NULL_HANDLE;
	bool initialized = false;
	bool Initialize(VkInstance vulkan_instance, WindowParameters window_parameters);
	void Release();
private:
	VkInstance vulkan_instance = VK_NULL_HANDLE;
};