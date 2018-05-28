#include "PhysicalDeviceFinder.h"
#include "VulkanFunctions.h"

PhysicalDeviceFinder::PhysicalDeviceFinder(VkInstance vulkan_instance)
{
	this->vulkan_instance = vulkan_instance;
}

std::vector<PhysicalDevice> PhysicalDeviceFinder::GetAllPhysicalDevices()
{
	std::vector<VkPhysicalDevice> available_devices;
	std::vector<PhysicalDevice> physical_devices;
	uint32_t devices_count = 0;
	VkResult result = VK_SUCCESS;

	//Get available physical devices
	result = PVF::vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, nullptr);
	if ((result != VK_SUCCESS) || (devices_count == 0))
	{
		std::cerr << "Could not get the number of available physical devices." << std::endl;
		return physical_devices;
	}

	available_devices.resize(devices_count);
	result = PVF::vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, available_devices.data());
	if ((result != VK_SUCCESS) || (devices_count == 0))
	{
		std::cerr << "Could not enumerate physical devices." << std::endl;
		return physical_devices;
	}

	for (auto & device_handle : available_devices)
	{
		PhysicalDevice new_device;
		if (new_device.Initialize(vulkan_instance, device_handle))
		{
			physical_devices.emplace_back(new_device);
		}
	}

	for (int i = physical_devices.size() - 1; i >= 0; i--)
	{
		if (!physical_devices[i].initialized)
		{
			physical_devices.erase(physical_devices.begin() + i);
		}
	}

	return physical_devices;
}

std::vector<PhysicalDevice> PhysicalDeviceFinder::GetAllPhysicalDevicesWithSupportedFeatures(VkPhysicalDeviceFeatures requested_features, bool has_graphics_bit, bool has_compute_bit)
{
	std::vector<PhysicalDevice> devices = GetAllPhysicalDevices();

	for (int i = devices.size() - 1; i >= 0; i--)
	{
		const int value_count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);
		VkBool32 request_value = VK_FALSE;
		VkBool32 actual_value = VK_FALSE;
		bool deviceHasRequiredFeatures = true;
		for (int v = 0; v < value_count; v++)
		{
			VkBool32 * featureOffset = reinterpret_cast<VkBool32*>(&devices[i].features) + v;
			VkBool32 * requestOffset = reinterpret_cast<VkBool32*>(&requested_features) + v;
			memcpy((void*)&actual_value, featureOffset, sizeof(VkBool32));
			memcpy((void*)&request_value, requestOffset, sizeof(VkBool32));
			if (request_value == VK_TRUE && actual_value == VK_FALSE)
			{
				deviceHasRequiredFeatures = false;
				break;
			}
		}
		if (!deviceHasRequiredFeatures)
		{
			devices.erase(devices.begin() + i);
		}
	}

	if (has_graphics_bit)
	{
		for(int i=devices.size()-1; i>=0; i--)
		{
			if (!PVF::SelectIndexOfQueueFamilyWithDesiredCapabilities(devices[i].handle, VK_QUEUE_GRAPHICS_BIT, devices[i].graphics_queue.family_index))
			{
				devices.erase(devices.begin() + i);
			}
		}
	}

	if (has_compute_bit)
	{
		for (int i = devices.size() - 1; i >= 0; i--)
		{
			if (!PVF::SelectIndexOfQueueFamilyWithDesiredCapabilities(devices[i].handle, VK_QUEUE_COMPUTE_BIT, devices[i].compute_queue.family_index))
			{
				devices.erase(devices.begin() + i);
			}
		}
	}

	return devices;
}

std::vector<PhysicalDevice> PhysicalDeviceFinder::GetAllPhysicalDevicesWithSupportedFeaturesAndPresentationToSurface(VkPhysicalDeviceFeatures requested_features, bool has_graphics_bit, bool has_compute_bit, VkSurfaceKHR presentation_surface)
{
	std::vector<PhysicalDevice> devices = GetAllPhysicalDevicesWithSupportedFeatures(requested_features, has_graphics_bit, has_compute_bit);

	for (int i = devices.size() - 1; i >= 0; i--)
	{
		if (!PVF::SelectQueueFamilyThatSupportsPresentationToGivenSurface(devices[i].handle, presentation_surface, devices[i].presentation_queue.family_index))
		{
			devices.erase(devices.begin() + i);
		}
	}

	return devices;
}
