#pragma once
#include "CommandBuffers.h"
class CommandPool
{
public:
	bool Initialize(PhysicalDevice & physical_device, LogicalDevice & logical_device, Swapchain & swapchain, Pipeline & pipeline, Framebuffers & framebuffers, RenderPass & renderpass, VertexBuffer & vertex_buffer);
	bool InitializeCommandBuffers(LogicalDevice & logical_device, Swapchain & swapchain, Pipeline & pipeline, Framebuffers & framebuffers, RenderPass & renderpass, VertexBuffer & vertex_buffer, DescriptorSetLayout & descriptor_set_layout);
	void Release();
	bool initialized = false;
	bool buffers_initialized = false;
	VkCommandPool handle = VK_NULL_HANDLE;
	CommandBuffers command_buffers;
	VkQueue graphics_queue = VK_NULL_HANDLE;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
};