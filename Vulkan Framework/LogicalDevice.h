#pragma once
#include "PhysicalDevice.h"
#include "QueueInfo.h"

class LogicalDevice
{
public:
	bool Initialize(PhysicalDevice & physical_device, std::vector<const char*> desired_extensions, VkPhysicalDeviceFeatures desired_features);
	void Release();
	VkDevice handle = VK_NULL_HANDLE;
	bool initialized = false;
	bool WaitForAllSubmittedCommandsToFinish();
	const VkPhysicalDevice GetPhysicalDeviceHandle();
private:
	PhysicalDevice physical_device;
	bool CreateLogicalDevice(PhysicalDevice physical_device, std::vector<const char*> desired_extensions, VkPhysicalDeviceFeatures desired_features);
};