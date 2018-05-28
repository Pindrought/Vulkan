#include "Swapchain.h"
#include "VulkanFunctions.h"

bool Swapchain::Initialize(LogicalDevice & logical_device, const PresentationSurface & presentation_surface, VkImageUsageFlagBits image_usage_bits, VkExtent2D image_size)
{
	Release();
	this->extent = image_size;
	this->logical_device = logical_device.handle;


	VkPresentModeKHR desired_present_mode;
	VkPhysicalDevice physical_device = logical_device.GetPhysicalDeviceHandle();
	if (!PVF::SelectDesiredPresentationMode(physical_device, presentation_surface.handle, VK_PRESENT_MODE_MAILBOX_KHR, desired_present_mode))
	{
		std::cerr << "Failed to select desired presentation mode." << std::endl;
		return false;
	}

	VkSurfaceCapabilitiesKHR surface_capabilities;
	if (!PVF::GetCapabilitiesOfPresentationSurface(physical_device, presentation_surface.handle, surface_capabilities))
	{
		std::cerr << "Failed to get capabilities of presentation surface." << std::endl;
		return false;
	}

	uint32_t number_of_images;
	if (!PVF::SelectNumberOfSwapchainImages(surface_capabilities, number_of_images))
	{
		std::cerr << "Failed to get select number of swapchain images." << std::endl;
		return false;
	}

	if (!PVF::ChooseSizeOfSwapchainImages(surface_capabilities, image_size))
	{
		std::cerr << "Failed to choose size of swapchain images." << std::endl;
		return false;
	}
	if ((0 == image_size.width) || (0 == image_size.height)) //This could possibly happen if the application is minimized so don't force close
	{
		std::cerr << "Chosen image size or width = 0?." << std::endl;
		return true;
	}
	extent = image_size;

	VkImageUsageFlags image_usage;
	if (!PVF::SelectDesiredUsageScenariosOfSwapchainImages(surface_capabilities, image_usage_bits, image_usage))
	{
		std::cerr << "Failed to select desired usage scenarios of swapchain images." << std::endl;
		return false;
	}

	VkSurfaceTransformFlagBitsKHR surface_transform;
	PVF::SelectTransformationOfSwapchainImages(surface_capabilities, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, surface_transform);

	VkColorSpaceKHR image_color_space;
	VkSurfaceFormatKHR desired_surface_format;

	desired_surface_format.format = VK_FORMAT_R8G8B8A8_UNORM;
	desired_surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	if (!PVF::SelectFormatOfSwapchainImages(physical_device, presentation_surface.handle, desired_surface_format, image_format, image_color_space))
	{
		std::cerr << "Failed to select format of swapchain images." << std::endl;
		return false;
	}

	/*std::cout << "Desired Format: " << desired_surface_format.format << std::endl;
	std::cout << "Selected Format: " << image_format << std::endl;
	std::cout << "Desired Color Space: " << desired_surface_format.colorSpace << std::endl;
	std::cout << "Selected Color Space: " << image_color_space << std::endl;*/

	if (!PVF::CreateSwapchain(logical_device.handle, presentation_surface.handle, number_of_images, { image_format, image_color_space }, image_size, image_usage, surface_transform, desired_present_mode, old_swapchain, handle))
	{
		std::cerr << "Failed to create swap chain." << std::endl;
		return false;
	}

	if (!PVF::GetHandlesOfSwapchainImages(logical_device.handle, handle, images))
	{
		std::cerr << "Failed to get handle of swapchain images." << std::endl;
		return false;
	}

	image_views.clear();
	for (int i = 0; i < images.size(); i++)
	{
		image_views.push_back(VK_NULL_HANDLE);
		if (!PVF::CreateImageView(logical_device.handle, images[i], VK_IMAGE_VIEW_TYPE_2D, image_format, VK_IMAGE_ASPECT_COLOR_BIT, image_views[i]))
		{
			std::cerr << "Failed to create image view #" << (i+1) << std::endl;
			return false;
		}
	}

	initialized = true;
	return true;
}

void Swapchain::Release()
{
	for (int i = 0; i < image_views.size(); i++)
	{
		if (image_views[i] != VK_NULL_HANDLE)
			PVF::vkDestroyImageView(logical_device, image_views[i], nullptr);
	}
	image_views.clear();


	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroySwapchainKHR(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
		initialized = false;
	}
}
