#include "LogicalDevice.h"
#include "VulkanFunctions.h"
#include "VulkanFunctionLoader.h"


bool LogicalDevice::Initialize(PhysicalDevice & physical_device, std::vector<const char*> desired_extensions, VkPhysicalDeviceFeatures desired_features)
{
	if (initialized)
		return true;

	Release();

	if (!CreateLogicalDevice(physical_device, desired_extensions, desired_features))
	{
		std::cerr << "Logical device creation failed for physical device: " << physical_device.properties.deviceName << std::endl;
		return false;
	}

	if (!PVF::LoadDeviceLevelFunctions(handle, desired_extensions))
	{
		std::cerr << "Failed to load device level functions for logical device." << std::endl;
		return false;
	}

	PVF::vkGetDeviceQueue(handle, physical_device.graphics_queue.family_index, 0, &physical_device.graphics_queue.handle);
	PVF::vkGetDeviceQueue(handle, physical_device.compute_queue.family_index, 0, &physical_device.compute_queue.handle);
	PVF::vkGetDeviceQueue(handle, physical_device.presentation_queue.family_index, 0, &physical_device.presentation_queue.handle);

	initialized = true;
	return true;
}

void LogicalDevice::Release()
{
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyDevice(handle, nullptr);
		initialized = false;
		handle = VK_NULL_HANDLE;
	}
}

bool LogicalDevice::WaitForAllSubmittedCommandsToFinish()
{
	if (handle != VK_NULL_HANDLE)
	{
		VkResult result = PVF::vkDeviceWaitIdle(handle);
		if (VK_SUCCESS != result)
		{
			std::cout << "Waiting failed on device: " << physical_device.properties.deviceName << std::endl;
			return false;
		}
		return true;
	}
	return false;
}

const VkPhysicalDevice LogicalDevice::GetPhysicalDeviceHandle()
{
	return physical_device.handle;
}

bool LogicalDevice::CreateLogicalDevice(PhysicalDevice physical_device, std::vector<const char*> desired_extensions, VkPhysicalDeviceFeatures desired_features)
{
	this->physical_device = physical_device;
	desired_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	for (auto & extension : desired_extensions)
	{
		if (!physical_device.IsExtensionSupported(extension))
		{
			std::cerr << "Extension: " << extension << " is not supported by device: " << physical_device.properties.deviceName << std::endl;
			return false;
		}
	}

	if (physical_device.compute_queue.family_index == UINT_MAX || physical_device.graphics_queue.family_index == UINT_MAX || physical_device.presentation_queue.family_index == UINT_MAX)
	{
		std::cerr << "One or more family indices for physical device queues are null. Could not create logical device on physical device: " << physical_device.properties.deviceName << std::endl;
		return false;
	}

	std::vector<QueueInfo> queue_infos = { { physical_device.graphics_queue.family_index, {1.0f} } };
	if (physical_device.compute_queue.family_index != physical_device.graphics_queue.family_index)
	{
		queue_infos.push_back({ physical_device.compute_queue.family_index, {1.0f} });
	}
	if (physical_device.presentation_queue.family_index != physical_device.graphics_queue.family_index && physical_device.presentation_queue.family_index != physical_device.compute_queue.family_index)
	{
		queue_infos.push_back({ physical_device.presentation_queue.family_index,{ 1.0f } });
	}

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

	for (auto & info : queue_infos)
	{
		queue_create_infos.push_back({
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType                  sType
			nullptr,                                          // const void                     * pNext
			0,                                                // VkDeviceQueueCreateFlags         flags
			info.family_index,                                 // uint32_t                         queueFamilyIndex
			static_cast<uint32_t>(info.priorities.size()),    // uint32_t                         queueCount
			info.priorities.data()                            // const float                    * pQueuePriorities
		});
	};

	VkDeviceCreateInfo device_create_info =
	{
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // VkStructureType                  sType
		nullptr,                                            // const void                     * pNext
		0,                                                  // VkDeviceCreateFlags              flags
		static_cast<uint32_t>(queue_create_infos.size()),   // uint32_t                         queueCreateInfoCount
		queue_create_infos.data(),                          // const VkDeviceQueueCreateInfo  * pQueueCreateInfos
		0,                                                  // uint32_t                         enabledLayerCount
		nullptr,                                            // const char * const             * ppEnabledLayerNames
		static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                         enabledExtensionCount
		desired_extensions.data(),                          // const char * const             * ppEnabledExtensionNames
		&desired_features                                    // const VkPhysicalDeviceFeatures * pEnabledFeatures
	};

	VkResult result = PVF::vkCreateDevice(physical_device.handle, &device_create_info, nullptr, &handle);
	if ((result != VK_SUCCESS) || (handle == VK_NULL_HANDLE))
	{
		std::cerr << "Could not create logical device." << std::endl;
		return false;
	}

	return true;
}
