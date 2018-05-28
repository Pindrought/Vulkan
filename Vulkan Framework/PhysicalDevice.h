#pragma once
#include "QueueParameters.h"
#include <vector>

class PhysicalDevice
{
public:
	bool Initialize(VkInstance vulkan_instance, VkPhysicalDevice handle);
	bool IsExtensionSupported(char const * const extension);
	VkPhysicalDevice handle = VK_NULL_HANDLE;
	bool initialized = false;
	VkPhysicalDeviceFeatures features;
	VkPhysicalDeviceProperties properties;
	QueueParameters graphics_queue;
	QueueParameters compute_queue;
	QueueParameters presentation_queue;


private:
	std::vector<VkExtensionProperties> available_extensions;
	std::vector<VkQueueFamilyProperties> queue_family_properties;
	
	
	VkInstance vulkan_instance = VK_NULL_HANDLE;
};