#include "CommandBuffers.h"
#include "VulkanFunctions.h"

bool CommandBuffers::Initialize(VkCommandPool command_pool, LogicalDevice & logical_device, Swapchain & swapchain, Pipeline & pipeline, Framebuffers & framebuffers, RenderPass & renderpass, VertexBuffer & vertex_buffer)
{
	Release();

	this->command_pool = command_pool;
	this->logical_device = logical_device.handle;

	buffers.resize(framebuffers.buffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)buffers.size();

	if (PVF::vkAllocateCommandBuffers(logical_device.handle, &allocInfo, buffers.data()) != VK_SUCCESS) 
	{
		std::cerr << "Failed to allocate command buffers." << std::endl;
		return false;
	}

	for (size_t i = 0; i < buffers.size(); i++) 
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (PVF::vkBeginCommandBuffer(buffers[i], &beginInfo) != VK_SUCCESS) 
		{
			std::cerr << "Failed to begin recording command buffer at index: " << i << std::endl;
			return false;
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderpass.handle;
		renderPassInfo.framebuffer = framebuffers.buffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapchain.extent;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		PVF::vkCmdBeginRenderPass(buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		PVF::vkCmdBindPipeline(buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle);

		VkBuffer vertexBuffers[] = { vertex_buffer.handle };
		VkDeviceSize offsets[] = { 0 };
		PVF::vkCmdBindVertexBuffers(buffers[i], 0, 1, vertexBuffers, offsets);

		PVF::vkCmdDraw(buffers[i], vertex_buffer.vertex_count, 1, 0, 0);

		PVF::vkCmdEndRenderPass(buffers[i]);

		if (PVF::vkEndCommandBuffer(buffers[i]) != VK_SUCCESS) 
		{
			std::cerr << "Failed to record command buffer at index: " << i << std::endl;
			return false;
		}
	}

	initialized = true;
	return true;
}

void CommandBuffers::Release()
{
	initialized = false;
	if (buffers.size()>0)
	{
		PVF::vkFreeCommandBuffers(logical_device, command_pool, buffers.size(), buffers.data());
	}
	buffers.clear();
}
