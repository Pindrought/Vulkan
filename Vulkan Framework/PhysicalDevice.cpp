#include "PhysicalDevice.h"
#include "VulkanFunctions.h"


bool PhysicalDevice::Initialize(VkInstance vulkan_instance, VkPhysicalDevice handle)
{
	this->vulkan_instance = vulkan_instance;
	this->handle = handle;
	initialized = true;

	if (!PVF::CheckAvailableDeviceExtensions(handle, available_extensions))
	{
		std::cerr << "Failed to retrieve available device extensions for physical device." << std::endl;
		initialized = false;
	}

	if (!PVF::CheckAvailableQueueFamiliesAndTheirProperties(handle, queue_family_properties))
	{
		std::cerr << "Failed to check available queue familes and their properties for physical device." << std::endl;
		initialized = false;
	}

	PVF::vkGetPhysicalDeviceFeatures(handle, &features);
	PVF::vkGetPhysicalDeviceProperties(handle, &properties);
	return initialized;
}

bool PhysicalDevice::IsExtensionSupported(char const * const extension)
{
	for (auto & available_extension : available_extensions)
	{
		if (strstr(available_extension.extensionName, extension))
		{
			return true;
		}
	}
	return false;
}
