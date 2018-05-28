#pragma once
#include "LogicalDevice.h"
#include "PresentationSurface.h"


class Swapchain
{
public:

	bool Initialize(LogicalDevice & logical_device, const PresentationSurface & presentation_surface, VkImageUsageFlagBits image_usage_bits,VkExtent2D image_size);
	void Release();
	VkSwapchainKHR handle = VK_NULL_HANDLE;
	bool initialized = false;
	std::vector<VkImage> images;
	std::vector<VkImageView> image_views;
	VkExtent2D extent;
	VkFormat image_format;
private:
	VkSwapchainKHR old_swapchain = VK_NULL_HANDLE;
	VkDevice logical_device = VK_NULL_HANDLE;
};