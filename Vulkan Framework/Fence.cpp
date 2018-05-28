#include "Fence.h"
#include "VulkanFunctions.h"

bool Fence::Initialize(VkDevice logical_device, VkFenceCreateFlags flags)
{
	Release();
	this->logical_device = logical_device;
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = flags;

	if (PVF::vkCreateFence(logical_device, &fenceInfo, nullptr, &handle) != VK_SUCCESS)
	{
		std::cerr << "Failed to create fence." << std::endl;
		return false;
	}

	initialized = true;
	return true;
}

void Fence::Release()
{
	initialized = false;
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyFence(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
	}
}

bool Fence::WaitForFence(uint64_t timeout)
{
	auto result = PVF::vkWaitForFences(logical_device, 1, &handle, VK_TRUE, timeout);
	if (result != VK_SUCCESS)
	{
		if (result == VK_TIMEOUT)
		{
			std::cerr << "Waiting on fence failed due to timeout." << std::endl;
			return false;
		}
		else
		{
			std::cerr << "Waiting on fence resulted in error." << std::endl;
			return false;
		}
	}
	return true;
}

bool Fence::ResetFence()
{
	if (PVF::vkResetFences(logical_device, 1, &handle) != VK_SUCCESS)
	{
		std::cerr << "Failed to reset fence." << std::endl;
		return false;
	}
	return true;
}
