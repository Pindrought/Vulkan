#include "Framebuffers.h"
#include "VulkanFunctions.h"

bool Framebuffers::Initialize(Swapchain & swapchain, RenderPass & renderpass, LogicalDevice & logical_device)
{
	this->logical_device = logical_device.handle;
	Release();
	buffers.resize(swapchain.image_views.size());
	for (auto & buffer : buffers)
	{
		buffer = VK_NULL_HANDLE;
	}

	for (size_t i = 0; i < swapchain.image_views.size(); i++)
	{
		VkImageView attachments[] =
		{
			swapchain.image_views[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderpass.handle;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapchain.extent.width;
		framebufferInfo.height = swapchain.extent.height;
		framebufferInfo.layers = 1;

		if (PVF::vkCreateFramebuffer(logical_device.handle, &framebufferInfo, nullptr, &buffers[i]) != VK_SUCCESS)
		{
			std::cerr << "Failed to create frame buffer." << std::endl;
			return false;
		}
	}
	initialized = true;
	return true;
}

void Framebuffers::Release()
{
	initialized = false;
	for (auto buffer : buffers) 
	{
		if (buffer != VK_NULL_HANDLE)
		{
			PVF::vkDestroyFramebuffer(logical_device, buffer, nullptr);
		}
	}
	buffers.clear();
}
