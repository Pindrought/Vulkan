#include "Semaphore.h"
#include "VulkanFunctions.h"

bool Semaphore::Initialize(LogicalDevice & logical_device)
{
	Release();

	this->logical_device = logical_device.handle;
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (PVF::vkCreateSemaphore(logical_device.handle, &semaphoreInfo, nullptr, &handle) != VK_SUCCESS)
	{
		std::cerr << "Failed to create semaphore." << std::endl;
		return false;
	}
	initialized = true;
	return true;
}

void Semaphore::Release()
{
	initialized = false;
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroySemaphore(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
	}
}
