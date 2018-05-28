#include "CommandPool.h"
#include "VulkanFunctions.h"

bool CommandPool::Initialize(PhysicalDevice & physical_device, LogicalDevice & logical_device, Swapchain & swapchain, Pipeline & pipeline, Framebuffers & framebuffers, RenderPass & renderpass, VertexBuffer & vertex_buffer)
{
	Release();
	this->logical_device = logical_device.handle;
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = physical_device.graphics_queue.family_index;

	if (PVF::vkCreateCommandPool(logical_device.handle, &poolInfo, nullptr, &handle) != VK_SUCCESS) 
	{
		std::cerr << "Failed to create command pool." << std::endl;
		return false;
	}

	if (!command_buffers.Initialize(handle, logical_device, swapchain, pipeline, framebuffers, renderpass, vertex_buffer))
	{
		std::cerr << "Failed to initialize command buffers." << std::endl;
		return false;
	}

	initialized = true;
	return true;
}

void CommandPool::Release()
{
	initialized = false;

	command_buffers.Release();

	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyCommandPool(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
	}
}
