#pragma once
#include "PhysicalDevice.h"

class PhysicalDeviceFinder
{
public:
	PhysicalDeviceFinder(VkInstance vulkan_instance);
	std::vector<PhysicalDevice> GetAllPhysicalDevices();
	std::vector<PhysicalDevice> GetAllPhysicalDevicesWithSupportedFeatures(VkPhysicalDeviceFeatures requested_features, bool has_graphics_bit = true, bool has_compute_bit = true);
	std::vector<PhysicalDevice> GetAllPhysicalDevicesWithSupportedFeaturesAndPresentationToSurface(VkPhysicalDeviceFeatures requested_features, bool has_graphics_bit, bool has_compute_bit, VkSurfaceKHR presentation_surface);

private:
	VkInstance vulkan_instance = VK_NULL_HANDLE;
	
};