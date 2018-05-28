#pragma once
#include "vulkan\vulkan.h"
#include <vector>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class DebugReport
{
public:
	bool Initialize(VkInstance vulkan_instance);
	void Release();
	bool initialized = false;
	VkDebugReportCallbackEXT callback = VK_NULL_HANDLE;
	const std::vector<const char*> validation_layers =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};
private:
	VkInstance vulkan_instance = VK_NULL_HANDLE;
};