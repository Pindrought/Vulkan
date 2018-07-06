#pragma once
#include "Pipeline.h"
#include "Framebuffers.h"
#include "VertexBuffer.h"
#include "DescriptorSetLayout.h"

class CommandBuffers
{
public:
	bool Initialize(VkCommandPool command_pool, LogicalDevice & logical_device, Swapchain & swapchain, Pipeline & pipeline, Framebuffers & framebuffers, RenderPass & renderpass, VertexBuffer & vertex_buffer, DescriptorSetLayout & descriptor_set_layout);
	void Release();
	bool initialized = false;
	std::vector<VkCommandBuffer> buffers;
private:
	VkCommandPool command_pool = VK_NULL_HANDLE;
	VkDevice logical_device = VK_NULL_HANDLE;
};