#pragma once
#include "RenderPass.h"
class Framebuffers
{
public:
	bool Initialize(Swapchain & swapchain, RenderPass & renderpass, LogicalDevice & logical_device);
	void Release();
	std::vector<VkFramebuffer> buffers;
	bool initialized = false;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
};