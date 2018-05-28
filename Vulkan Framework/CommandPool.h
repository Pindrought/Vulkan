#pragma once
#include "CommandBuffers.h"
class CommandPool
{
public:
	bool Initialize(PhysicalDevice & physical_device, LogicalDevice & logical_device, Swapchain & swapchain, Pipeline & pipeline, Framebuffers & framebuffers, RenderPass & renderpass, VertexBuffer & vertex_buffer);
	void Release();
	bool initialized = false;
	VkCommandPool handle = VK_NULL_HANDLE;
	CommandBuffers command_buffers;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
};