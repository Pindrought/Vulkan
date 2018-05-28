#pragma once
#include "Swapchain.h"
class RenderPass
{
public:
	bool Initialize(LogicalDevice & logical_device, Swapchain & swapchain);
	void Release();
	bool initialized = false;
	VkRenderPass handle = VK_NULL_HANDLE;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
};