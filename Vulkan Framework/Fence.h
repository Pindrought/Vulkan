#pragma once
#include "vulkan\vulkan.h"
class Fence
{
public:
	bool Initialize(VkDevice logical_device, VkFenceCreateFlags flags = VK_FENCE_CREATE_SIGNALED_BIT);
	void Release();
	bool WaitForFence(uint64_t timeout);
	bool ResetFence();
	bool initialized = false;
	VkFence handle = VK_NULL_HANDLE;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
};