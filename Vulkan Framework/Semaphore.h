#pragma once
#include "LogicalDevice.h"
class Semaphore
{
public:
	bool Initialize(LogicalDevice & logical_device);
	void Release();
	bool initialized = false;
	VkSemaphore handle = VK_NULL_HANDLE;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
};