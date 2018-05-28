#include "VulkanFunctions.h"

namespace PVF
{
	#define EXPORTED_VULKAN_FUNCTION( name ) PFN_##name name;
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;

	#include "ListOfVulkanFunctions.inl"


/*
	bool CheckAvailableInstanceExtensions(std::vector<VkExtensionProperties> & available_extensions) 
	{
		uint32_t extensions_count = 0;
		VkResult result = VK_SUCCESS;

		result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr);
		if ((result != VK_SUCCESS) || (extensions_count == 0)) 
		{
			std::cerr << "Could not get the number of instance extensions." << std::endl;
			return false;
		}

		available_extensions.resize(extensions_count);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, available_extensions.data());
		if ((result != VK_SUCCESS) || (extensions_count == 0)) 
		{
			std::cerr << "Could not enumerate instance extensions." << std::endl;
			return false;
		}

		return true;
	}*/

	bool IsExtensionSupported(	std::vector<VkExtensionProperties> const & available_extensions,
								char const * const                         extension) 
	{
		for (auto & available_extension : available_extensions) 
		{
			if (strstr(available_extension.extensionName, extension)) 
			{
				return true;
			}
		}
		return false;
	}

	//bool CreateVulkanInstance(	std::vector<char const *> const & desired_extensions,
	//							char const * const                application_name,
	//							VkInstance                      & instance) 
	//{
	//	std::vector<VkExtensionProperties> available_extensions;
	//	if (!CheckAvailableInstanceExtensions(available_extensions)) 
	//	{
	//		return false;
	//	}

	//	for (auto & extension : desired_extensions) 
	//	{
	//		if (!IsExtensionSupported(available_extensions, extension)) 
	//		{
	//			std::cerr << "Extension named '" << extension << "' is not supported by an Instance object." << std::endl;
	//			return false;
	//		}
	//	}

	//	VkApplicationInfo application_info = 
	//	{
	//		VK_STRUCTURE_TYPE_APPLICATION_INFO,                 // VkStructureType           sType
	//		nullptr,                                            // const void              * pNext
	//		application_name,                                   // const char              * pApplicationName
	//		VK_MAKE_VERSION(1, 0, 0),							// uint32_t                  applicationVersion
	//		"PVF Vulkan Engine",                                // const char              * pEngineName
	//		VK_MAKE_VERSION(1, 0, 0),							// uint32_t                  engineVersion
	//		VK_MAKE_VERSION(1, 0, 0)							// uint32_t                  apiVersion
	//	};

	//	VkInstanceCreateInfo instance_create_info = 
	//	{
	//		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,             // VkStructureType           sType
	//		nullptr,                                            // const void              * pNext
	//		0,                                                  // VkInstanceCreateFlags     flags
	//		&application_info,                                  // const VkApplicationInfo * pApplicationInfo
	//		0,                                                  // uint32_t                  enabledLayerCount
	//		nullptr,                                            // const char * const      * ppEnabledLayerNames
	//		static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                  enabledExtensionCount
	//		desired_extensions.data()                           // const char * const      * ppEnabledExtensionNames
	//	};

	//	VkResult result = vkCreateInstance(&instance_create_info, nullptr, &instance);
	//	if ((result != VK_SUCCESS) || (instance == VK_NULL_HANDLE)) 
	//	{
	//		std::cerr << "Could not create Vulkan instance." << std::endl;
	//		return false;
	//	}

	//	return true;
	//}

	/*bool CreateVulkanInstanceWithWsiExtensionsEnabled(	std::vector<char const *> & desired_extensions,
														char const * const          application_name,
														VkInstance                & instance) 
	{

		desired_extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		desired_extensions.emplace_back(
							#ifdef VK_USE_PLATFORM_WIN32_KHR
										VK_KHR_WIN32_SURFACE_EXTENSION_NAME

							#elif defined VK_USE_PLATFORM_XCB_KHR
										VK_KHR_XCB_SURFACE_EXTENSION_NAME

							#elif defined VK_USE_PLATFORM_XLIB_KHR
										VK_KHR_XLIB_SURFACE_EXTENSION_NAME
							#endif
		);

		return CreateVulkanInstance(desired_extensions, application_name, instance);
	}*/

	bool EnumerateAvailablePhysicalDevices(VkInstance                      instance,
											std::vector<VkPhysicalDevice> & available_devices) 
	{
		uint32_t devices_count = 0;
		VkResult result = VK_SUCCESS;

		result = vkEnumeratePhysicalDevices(instance, &devices_count, nullptr);
		if ((result != VK_SUCCESS) || (devices_count == 0)) 
		{
			std::cerr << "Could not get the number of available physical devices." << std::endl;
			return false;
		}

		available_devices.resize(devices_count);
		result = vkEnumeratePhysicalDevices(instance, &devices_count, available_devices.data());
		if ((result != VK_SUCCESS) || (devices_count == 0)) 
		{
			std::cerr << "Could not enumerate physical devices." << std::endl;
			return false;
		}

		return true;
	}

	bool CheckAvailableDeviceExtensions(VkPhysicalDevice                     physical_device,
										std::vector<VkExtensionProperties> & available_extensions) 
	{
		uint32_t extensions_count = 0;
		VkResult result = VK_SUCCESS;

		result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr);
		if ((result != VK_SUCCESS) || (extensions_count == 0))
		{
			std::cerr << "Could not get the number of device extensions." << std::endl;
			return false;
		}

		available_extensions.resize(extensions_count);
		result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, available_extensions.data());
		if ((result != VK_SUCCESS) || (extensions_count == 0))
		{
			std::cerr << "Could not enumerate device extensions." << std::endl;
			return false;
		}

		return true;
	}

	void GetFeaturesAndPropertiesOfPhysicalDevice(	VkPhysicalDevice             physical_device,
													VkPhysicalDeviceFeatures   & device_features,
													VkPhysicalDeviceProperties & device_properties) 
	{
		vkGetPhysicalDeviceFeatures(physical_device, &device_features);

		vkGetPhysicalDeviceProperties(physical_device, &device_properties);
	}

	bool CheckAvailableQueueFamiliesAndTheirProperties(	VkPhysicalDevice                       physical_device,
														std::vector<VkQueueFamilyProperties> & queue_families) 
	{
		uint32_t queue_families_count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, nullptr);
		if (queue_families_count == 0) 
		{
			std::cerr << "Could not get the number of queue families." << std::endl;
			return false;
		}

		queue_families.resize(queue_families_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, queue_families.data());
		if (queue_families_count == 0) 
		{
			std::cerr << "Could not acquire properties of queue families." << std::endl;
			return false;
		}

		return true;
	}

	bool SelectIndexOfQueueFamilyWithDesiredCapabilities(	VkPhysicalDevice   physical_device,
															VkQueueFlags       desired_capabilities,
															uint32_t         & queue_family_index) 
	{
		std::vector<VkQueueFamilyProperties> queue_families;
		if (!CheckAvailableQueueFamiliesAndTheirProperties(physical_device, queue_families)) 
		{
			return false;
		}

		for (uint32_t index = 0; index < static_cast<uint32_t>(queue_families.size()); ++index) 
		{
			if ((queue_families[index].queueCount > 0) && (queue_families[index].queueFlags & desired_capabilities)) 
			{
				queue_family_index = index;
				return true;
			}
		}
		return false;
	}

	bool CreateLogicalDevice(	VkPhysicalDevice                  physical_device,
								std::vector<QueueInfo>            queue_infos,
								std::vector<char const *> const & desired_extensions,
								VkPhysicalDeviceFeatures        * desired_features,
								VkDevice                        & logical_device) 
	{
		std::vector<VkExtensionProperties> available_extensions;
		if (!CheckAvailableDeviceExtensions(physical_device, available_extensions)) 
		{
			return false;
		}

		for (auto & extension : desired_extensions) 
		{
			if (!IsExtensionSupported(available_extensions, extension)) 
			{
				std::cerr << "Extension named '" << extension << "' is not supported by a physical device." << std::endl;
				return false;
			}
		}

		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

		for (auto & info : queue_infos) 
		{
			queue_create_infos.push_back({
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType                  sType
				nullptr,                                          // const void                     * pNext
				0,                                                // VkDeviceQueueCreateFlags         flags
				info.FamilyIndex,                                 // uint32_t                         queueFamilyIndex
				static_cast<uint32_t>(info.Priorities.size()),    // uint32_t                         queueCount
				info.Priorities.data()                            // const float                    * pQueuePriorities
			});
		};

		VkDeviceCreateInfo device_create_info = 
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // VkStructureType                  sType
			nullptr,                                            // const void                     * pNext
			0,                                                  // VkDeviceCreateFlags              flags
			static_cast<uint32_t>(queue_create_infos.size()),   // uint32_t                         queueCreateInfoCount
			queue_create_infos.data(),                          // const VkDeviceQueueCreateInfo  * pQueueCreateInfos
			0,                                                  // uint32_t                         enabledLayerCount
			nullptr,                                            // const char * const             * ppEnabledLayerNames
			static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                         enabledExtensionCount
			desired_extensions.data(),                          // const char * const             * ppEnabledExtensionNames
			desired_features                                    // const VkPhysicalDeviceFeatures * pEnabledFeatures
		};

		VkResult result = vkCreateDevice(physical_device, &device_create_info, nullptr, &logical_device);
		if ((result != VK_SUCCESS) || (logical_device == VK_NULL_HANDLE)) 
		{
			std::cerr << "Could not create logical device." << std::endl;
			return false;
		}

		return true;
	}

	bool CreateLogicalDeviceWithWsiExtensionsEnabled(	VkPhysicalDevice            physical_device,
														std::vector< QueueInfo >    queue_infos,
														std::vector<char const *> & desired_extensions,
														VkPhysicalDeviceFeatures  * desired_features,
														VkDevice                  & logical_device) 
	{
		desired_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		return CreateLogicalDevice(physical_device, queue_infos, desired_extensions, desired_features, logical_device);
	}

	bool CreatePresentationSurface(	VkInstance         instance,
									WindowParameters   window_parameters,
									VkSurfaceKHR     & presentation_surface) 
	{
		VkResult result;

		#ifdef VK_USE_PLATFORM_WIN32_KHR

			VkWin32SurfaceCreateInfoKHR surface_create_info = 
			{
				VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                 sType
				nullptr,                                          // const void                    * pNext
				0,                                                // VkWin32SurfaceCreateFlagsKHR    flags
				window_parameters.HInstance,                      // HINSTANCE                       hinstance
				window_parameters.HWnd                            // HWND                            hwnd
			};

			result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &presentation_surface);

		#elif defined VK_USE_PLATFORM_XLIB_KHR

			VkXlibSurfaceCreateInfoKHR surface_create_info = 
			{
				VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,   // VkStructureType                 sType
				nullptr,                                          // const void                    * pNext
				0,                                                // VkXlibSurfaceCreateFlagsKHR     flags
				window_parameters.Dpy,                            // Display                       * dpy
				window_parameters.Window                          // Window                          window
			};

			result = vkCreateXlibSurfaceKHR(instance, &surface_create_info, nullptr, &presentation_surface);

		#elif defined VK_USE_PLATFORM_XCB_KHR

			VkXcbSurfaceCreateInfoKHR surface_create_info = 
			{
				VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,    // VkStructureType                 sType
				nullptr,                                          // const void                    * pNext
				0,                                                // VkXcbSurfaceCreateFlagsKHR      flags
				window_parameters.Connection,                     // xcb_connection_t              * connection
				window_parameters.Window                          // xcb_window_t                    window
			};

			result = vkCreateXcbSurfaceKHR(instance, &surface_create_info, nullptr, &presentation_surface);

		#endif

		if ((VK_SUCCESS != result) || (VK_NULL_HANDLE == presentation_surface))
		{
			std::cerr << "Could not create presentation surface." << std::endl;
			return false;
		}
		return true;
	}

	bool SelectQueueFamilyThatSupportsPresentationToGivenSurface(	VkPhysicalDevice   physical_device,
																	VkSurfaceKHR       presentation_surface,
																	uint32_t         & queue_family_index) 
	{
		std::vector<VkQueueFamilyProperties> queue_families;
		if (!CheckAvailableQueueFamiliesAndTheirProperties(physical_device, queue_families)) 
		{
			return false;
		}

		for (uint32_t index = 0; index < static_cast<uint32_t>(queue_families.size()); ++index) 
		{
			VkBool32 presentation_supported = VK_FALSE;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, presentation_surface, &presentation_supported);
			if ((VK_SUCCESS == result) && (VK_TRUE == presentation_supported))
			{
				queue_family_index = index;
				return true;
			}
		}
		return false;
	}

	bool SelectDesiredPresentationMode(	VkPhysicalDevice   physical_device,
										VkSurfaceKHR       presentation_surface,
										VkPresentModeKHR   desired_present_mode,
										VkPresentModeKHR & present_mode) 
	{
		// Enumerate supported present modes
		uint32_t present_modes_count = 0;
		VkResult result = VK_SUCCESS;

		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, presentation_surface, &present_modes_count, nullptr);
		if ((VK_SUCCESS != result) || (0 == present_modes_count))
		{
			std::cout << "Could not get the number of supported present modes." << std::endl;
			return false;
		}

		std::vector<VkPresentModeKHR> present_modes(present_modes_count);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, presentation_surface, &present_modes_count, present_modes.data());
		if ((VK_SUCCESS != result) || (0 == present_modes_count))
		{
			std::cout << "Could not enumerate present modes." << std::endl;
			return false;
		}

		// Select present mode
		for (auto & current_present_mode : present_modes) 
		{
			if (current_present_mode == desired_present_mode) 
			{
				present_mode = desired_present_mode;
				return true;
			}
		}

		std::cout << "Desired present mode is not supported. Selecting default FIFO mode." << std::endl;
		for (auto & current_present_mode : present_modes) 
		{
			if (current_present_mode == VK_PRESENT_MODE_FIFO_KHR) 
			{
				present_mode = VK_PRESENT_MODE_FIFO_KHR;
				return true;
			}
		}

		std::cout << "VK_PRESENT_MODE_FIFO_KHR is not supported though it's mandatory for all drivers!" << std::endl;
		return false;
	}

	bool GetCapabilitiesOfPresentationSurface(	VkPhysicalDevice           physical_device,
												VkSurfaceKHR               presentation_surface,
												VkSurfaceCapabilitiesKHR & surface_capabilities) 
	{
		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, presentation_surface, &surface_capabilities);

		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not get the capabilities of a presentation surface." << std::endl;
			return false;
		}
		return true;
	}

	bool SelectNumberOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
										uint32_t                       & number_of_images)
	{
		number_of_images = surface_capabilities.minImageCount + 1;
		if ((surface_capabilities.maxImageCount > 0) && (number_of_images > surface_capabilities.maxImageCount))
		{
			number_of_images = surface_capabilities.maxImageCount;
		}
		return true;
	}

	bool ChooseSizeOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
										VkExtent2D                     & size_of_images) 
	{
		if (0xFFFFFFFF == surface_capabilities.currentExtent.width) 
		{
			size_of_images = { 640, 480 };

			if (size_of_images.width < surface_capabilities.minImageExtent.width) 
			{
				size_of_images.width = surface_capabilities.minImageExtent.width;
			}
			else if (size_of_images.width > surface_capabilities.maxImageExtent.width) 
			{
				size_of_images.width = surface_capabilities.maxImageExtent.width;
			}

			if (size_of_images.height < surface_capabilities.minImageExtent.height) 
			{
				size_of_images.height = surface_capabilities.minImageExtent.height;
			}
			else if (size_of_images.height > surface_capabilities.maxImageExtent.height) 
			{
				size_of_images.height = surface_capabilities.maxImageExtent.height;
			}
		}
		else 
		{
			size_of_images = surface_capabilities.currentExtent;
		}
		return true;
	}

	bool SelectDesiredUsageScenariosOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
														VkImageUsageFlags                desired_usages,
														VkImageUsageFlags              & image_usage) 
	{
		image_usage = desired_usages & surface_capabilities.supportedUsageFlags;

		return desired_usages == image_usage;
	}

	bool SelectTransformationOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
												VkSurfaceTransformFlagBitsKHR    desired_transform,
												VkSurfaceTransformFlagBitsKHR  & surface_transform) 
	{
		if (surface_capabilities.supportedTransforms & desired_transform) 
		{
			surface_transform = desired_transform;
		}
		else 
		{
			surface_transform = surface_capabilities.currentTransform;
		}
		return true;
	}

	bool SelectFormatOfSwapchainImages(	VkPhysicalDevice     physical_device,
										VkSurfaceKHR         presentation_surface,
										VkSurfaceFormatKHR   desired_surface_format,
										VkFormat           & image_format,
										VkColorSpaceKHR    & image_color_space) 
	{
		// Enumerate supported formats
		uint32_t formats_count = 0;
		VkResult result = VK_SUCCESS;

		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, presentation_surface, &formats_count, nullptr);
		if ((VK_SUCCESS != result) || (0 == formats_count))
		{
			std::cout << "Could not get the number of supported surface formats." << std::endl;
			return false;
		}

		std::vector<VkSurfaceFormatKHR> surface_formats(formats_count);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, presentation_surface, &formats_count, surface_formats.data());
		if ((VK_SUCCESS != result) || (0 == formats_count))
		{
			std::cout << "Could not enumerate supported surface formats." << std::endl;
			return false;
		}

		// Select surface format
		if ((1 == surface_formats.size()) && (VK_FORMAT_UNDEFINED == surface_formats[0].format))
		{
			image_format = desired_surface_format.format;
			image_color_space = desired_surface_format.colorSpace;
			return true;
		}

		for (auto & surface_format : surface_formats) 
		{
			if ((desired_surface_format.format == surface_format.format) && (desired_surface_format.colorSpace == surface_format.colorSpace))
			{
				image_format = desired_surface_format.format;
				image_color_space = desired_surface_format.colorSpace;
				return true;
			}
		}

		for (auto & surface_format : surface_formats) 
		{
			if ((desired_surface_format.format == surface_format.format)) 
			{
				image_format = desired_surface_format.format;
				image_color_space = surface_format.colorSpace;
				//std::cout << "Desired combination of format and colorspace is not supported. Selecting other colorspace." << std::endl;
				return true;
			}
		}

		image_format = surface_formats[0].format;
		image_color_space = surface_formats[0].colorSpace;
		//std::cout << "Desired format is not supported. Selecting available format - colorspace combination." << std::endl;
		return true;
	}

	bool CreateSwapchain(	VkDevice                        logical_device,
							VkSurfaceKHR                    presentation_surface,
							uint32_t                        image_count,
							VkSurfaceFormatKHR              surface_format,
							VkExtent2D                      image_size,
							VkImageUsageFlags               image_usage,
							VkSurfaceTransformFlagBitsKHR   surface_transform,
							VkPresentModeKHR                present_mode,
							VkSwapchainKHR                & old_swapchain,
							VkSwapchainKHR                & swapchain) 
	{
		VkSwapchainCreateInfoKHR swapchain_create_info = 
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                  sType
			nullptr,                                      // const void                     * pNext
			0,                                            // VkSwapchainCreateFlagsKHR        flags
			presentation_surface,                         // VkSurfaceKHR                     surface
			image_count,                                  // uint32_t                         minImageCount
			surface_format.format,                        // VkFormat                         imageFormat
			surface_format.colorSpace,                    // VkColorSpaceKHR                  imageColorSpace
			image_size,                                   // VkExtent2D                       imageExtent
			1,                                            // uint32_t                         imageArrayLayers
			image_usage,                                  // VkImageUsageFlags                imageUsage
			VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                    imageSharingMode
			0,                                            // uint32_t                         queueFamilyIndexCount
			nullptr,                                      // const uint32_t                 * pQueueFamilyIndices
			surface_transform,                            // VkSurfaceTransformFlagBitsKHR    preTransform
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            // VkCompositeAlphaFlagBitsKHR      compositeAlpha
			present_mode,                                 // VkPresentModeKHR                 presentMode
			VK_TRUE,                                      // VkBool32                         clipped
			old_swapchain                                 // VkSwapchainKHR                   oldSwapchain
		};

		VkResult result = vkCreateSwapchainKHR(logical_device, &swapchain_create_info, nullptr, &swapchain);
		if ((VK_SUCCESS != result) || (VK_NULL_HANDLE == swapchain))
		{
			std::cout << "Could not create a swapchain." << std::endl;
			return false;
		}

		if (VK_NULL_HANDLE != old_swapchain) 
		{
			vkDestroySwapchainKHR(logical_device, old_swapchain, nullptr);
			old_swapchain = VK_NULL_HANDLE;
		}

		return true;
	}

	bool GetHandlesOfSwapchainImages(	VkDevice               logical_device,
										VkSwapchainKHR         swapchain,
										std::vector<VkImage> & swapchain_images) 
	{
		uint32_t images_count = 0;
		VkResult result = VK_SUCCESS;

		result = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, nullptr);
		if ((VK_SUCCESS != result) || (0 == images_count))
		{
			std::cout << "Could not get the number of swapchain images." << std::endl;
			return false;
		}

		swapchain_images.resize(images_count);
		result = vkGetSwapchainImagesKHR(logical_device, swapchain, &images_count, swapchain_images.data());
		if ((VK_SUCCESS != result) || (0 == images_count))
		{
			std::cout << "Could not enumerate swapchain images." << std::endl;
			return false;
		}

		return true;
	}

	bool CreateSwapchainWithR8G8B8A8FormatAndMailboxPresentMode(VkPhysicalDevice       physical_device,
																VkSurfaceKHR           presentation_surface,
																VkDevice               logical_device,
																VkImageUsageFlags      swapchain_image_usage,
																VkExtent2D           & image_size,
																VkFormat             & image_format,
																VkSwapchainKHR       & old_swapchain,
																VkSwapchainKHR       & swapchain,
																std::vector<VkImage> & swapchain_images) 
	{
		VkPresentModeKHR desired_present_mode;
		if (!PVF::SelectDesiredPresentationMode(physical_device, presentation_surface, VK_PRESENT_MODE_MAILBOX_KHR, desired_present_mode))
		{
			std::cerr << "Failed to select desired presentation mode." << std::endl;
			return false;
		}

		VkSurfaceCapabilitiesKHR surface_capabilities;
		if (!PVF::GetCapabilitiesOfPresentationSurface(physical_device, presentation_surface, surface_capabilities))
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

		VkImageUsageFlags image_usage;
		if (!PVF::SelectDesiredUsageScenariosOfSwapchainImages(surface_capabilities, swapchain_image_usage, image_usage))
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
		if (!PVF::SelectFormatOfSwapchainImages(physical_device, presentation_surface, desired_surface_format, image_format, image_color_space))
		{
			std::cerr << "Failed to select format of swapchain images." << std::endl;
			return false;
		}

		std::cout << "Desired Format: " << desired_surface_format.format << std::endl;
		std::cout << "Selected Format: " << image_format << std::endl;
		std::cout << "Desired Color Space: " << desired_surface_format.colorSpace << std::endl;
		std::cout << "Selected Color Space: " << image_color_space << std::endl;


		if (!PVF::CreateSwapchain(logical_device, presentation_surface, number_of_images, { image_format, image_color_space }, image_size, image_usage, surface_transform, desired_present_mode, old_swapchain, swapchain))
		{
			std::cerr << "Failed to create swap chain." << std::endl;
			return false;
		}

		if (!GetHandlesOfSwapchainImages(logical_device, swapchain, swapchain_images)) 
		{
			std::cerr << "Failed to get handle of swapchain images." << std::endl;
			return false;
		}

		return true;
	}

	bool CreateCommandPool(	VkDevice                   logical_device,
							VkCommandPoolCreateFlags   parameters,
							uint32_t                   queue_family,
							VkCommandPool            & command_pool) 
	{
		VkCommandPoolCreateInfo command_pool_create_info = 
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,   // VkStructureType              sType
			nullptr,                                      // const void                 * pNext
			parameters,                                   // VkCommandPoolCreateFlags     flags
			queue_family                                  // uint32_t                     queueFamilyIndex
		};

		VkResult result = vkCreateCommandPool(logical_device, &command_pool_create_info, nullptr, &command_pool);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create command pool." << std::endl;
			return false;
		}

		return true;
	}

	bool AllocateCommandBuffers(VkDevice                       logical_device,
								VkCommandPool                  command_pool,
								VkCommandBufferLevel           level,
								uint32_t                       count,
								std::vector<VkCommandBuffer> & command_buffers) 
	{
		VkCommandBufferAllocateInfo command_buffer_allocate_info = 
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,   // VkStructureType          sType
			nullptr,                                          // const void             * pNext
			command_pool,                                     // VkCommandPool            commandPool
			level,                                            // VkCommandBufferLevel     level
			count                                             // uint32_t                 commandBufferCount
		};

		command_buffers.resize(count);

		VkResult result = vkAllocateCommandBuffers(logical_device, &command_buffer_allocate_info, command_buffers.data());
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not allocate command buffers." << std::endl;
			return false;
		}
		return true;
	}

	bool BeginCommandBufferRecordingOperation(	VkCommandBuffer                  command_buffer,
												VkCommandBufferUsageFlags        usage,
												VkCommandBufferInheritanceInfo * secondary_command_buffer_info) 
	{
		VkCommandBufferBeginInfo command_buffer_begin_info = 
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,    // VkStructureType                        sType
			nullptr,                                        // const void                           * pNext
			usage,                                          // VkCommandBufferUsageFlags              flags
			secondary_command_buffer_info                   // const VkCommandBufferInheritanceInfo * pInheritanceInfo
		};

		VkResult result = vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not begin command buffer recording operation." << std::endl;
			return false;
		}
		return true;
	}

	bool EndCommandBufferRecordingOperation(VkCommandBuffer command_buffer) 
	{
		VkResult result = vkEndCommandBuffer(command_buffer);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Error occurred during command buffer recording." << std::endl;
			return false;
		}
		return true;
	}

	bool CreateSemaphore(	VkDevice      logical_device,
							VkSemaphore & semaphore) 
	{
		VkSemaphoreCreateInfo semaphore_create_info = 
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,    // VkStructureType            sType
			nullptr,                                    // const void               * pNext
			0                                           // VkSemaphoreCreateFlags     flags
		};

		VkResult result = vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, &semaphore);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a semaphore." << std::endl;
			return false;
		}
		return true;
	}

	bool CreateFence(	VkDevice   logical_device,
						bool       signaled,
						VkFence  & fence) 
	{
		VkFenceCreateInfo fence_create_info = 
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,          // VkStructureType        sType
			nullptr,                                      // const void           * pNext
			signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0u, // VkFenceCreateFlags     flags
		};

		VkResult result = vkCreateFence(logical_device, &fence_create_info, nullptr, &fence);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a fence." << std::endl;
			return false;
		}
		return true;
	}

	bool WaitForFences(	VkDevice                     logical_device,
						std::vector<VkFence> const & fences,
						VkBool32                     wait_for_all,
						uint64_t                     timeout) 
	{
		if (fences.size() > 0) 
		{
			VkResult result = vkWaitForFences(logical_device, static_cast<uint32_t>(fences.size()), fences.data(), wait_for_all, timeout);
			if (VK_SUCCESS != result) 
			{
				std::cout << "Waiting on fence failed." << std::endl;
				return false;
			}
			return true;
		}
		return false;
	}

	bool ResetFences(	VkDevice                     logical_device,
						std::vector<VkFence> const & fences) 
	{
		if (fences.size() > 0) 
		{
			VkResult result = vkResetFences(logical_device, static_cast<uint32_t>(fences.size()), fences.data());
			if (VK_SUCCESS != result) 
			{
				std::cout << "Error occurred when tried to reset fences." << std::endl;
				return false;
			}
			return VK_SUCCESS == result;
		}
		return false;
	}

	bool SubmitCommandBuffersToQueue(	VkQueue                         queue,
										std::vector<WaitSemaphoreInfo>  wait_semaphore_infos,
										std::vector<VkCommandBuffer>    command_buffers,
										std::vector<VkSemaphore>        signal_semaphores,
										VkFence                         fence) 
	{
		std::vector<VkSemaphore>          wait_semaphore_handles;
		std::vector<VkPipelineStageFlags> wait_semaphore_stages;

		for (auto & wait_semaphore_info : wait_semaphore_infos) 
		{
			wait_semaphore_handles.emplace_back(wait_semaphore_info.Semaphore);
			wait_semaphore_stages.emplace_back(wait_semaphore_info.WaitingStage);
		}

		VkSubmitInfo submit_info = 
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,                        // VkStructureType                sType
			nullptr,                                              // const void                   * pNext
			static_cast<uint32_t>(wait_semaphore_infos.size()),   // uint32_t                       waitSemaphoreCount
			wait_semaphore_handles.data(),                        // const VkSemaphore            * pWaitSemaphores
			wait_semaphore_stages.data(),                         // const VkPipelineStageFlags   * pWaitDstStageMask
			static_cast<uint32_t>(command_buffers.size()),        // uint32_t                       commandBufferCount
			command_buffers.data(),                               // const VkCommandBuffer        * pCommandBuffers
			static_cast<uint32_t>(signal_semaphores.size()),      // uint32_t                       signalSemaphoreCount
			signal_semaphores.data()                              // const VkSemaphore            * pSignalSemaphores
		};

		VkResult result = vkQueueSubmit(queue, 1, &submit_info, fence);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Error occurred during command buffer submission." << std::endl;
			return false;
		}
		return true;
	}

	bool WaitForAllSubmittedCommandsToBeFinished(VkDevice logical_device) 
	{
		VkResult result = vkDeviceWaitIdle(logical_device);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Waiting on a device failed." << std::endl;
			return false;
		}
		return true;
	}

	bool CreateImageView(	VkDevice             logical_device,
							VkImage              image,
							VkImageViewType      view_type,
							VkFormat             format,
							VkImageAspectFlags   aspect,
							VkImageView        & image_view) 
	{
		VkImageViewCreateInfo image_view_create_info = 
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   // VkStructureType            sType
			nullptr,                                    // const void               * pNext
			0,                                          // VkImageViewCreateFlags     flags
			image,                                      // VkImage                    image
			view_type,                                  // VkImageViewType            viewType
			format,                                     // VkFormat                   format
			{                                           // VkComponentMapping         components
				VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         r
				VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         g
				VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         b
				VK_COMPONENT_SWIZZLE_IDENTITY               // VkComponentSwizzle         a
			},
			{                                           // VkImageSubresourceRange    subresourceRange
				aspect,                                     // VkImageAspectFlags         aspectMask
				0,                                          // uint32_t                   baseMipLevel
				VK_REMAINING_MIP_LEVELS,                    // uint32_t                   levelCount
				0,                                          // uint32_t                   baseArrayLayer
				VK_REMAINING_ARRAY_LAYERS                   // uint32_t                   layerCount
			}
		};

		VkResult result = vkCreateImageView(logical_device, &image_view_create_info, nullptr, &image_view);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create an image view." << std::endl;
			return false;
		}
		return true;
	}

	bool GetBinaryFileContents(	std::string const          & filename,
								std::vector<unsigned char> & contents)
	{
		contents.clear();

		std::ifstream file(filename, std::ios::binary);
		if (file.fail()) 
		{
			std::cout << "Could not open '" << filename << "' file." << std::endl;
			return false;
		}

		std::streampos begin;
		std::streampos end;
		begin = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();

		if ((end - begin) == 0) 
		{
			std::cout << "The '" << filename << "' file is empty." << std::endl;
			return false;
		}
		contents.resize(static_cast<size_t>(end - begin));
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(contents.data()), end - begin);
		file.close();

		return true;
	}

	bool CreateShaderModule(VkDevice                           logical_device,
							std::vector<unsigned char> const & source_code,
							VkShaderModule                   & shader_module) 
	{
		VkShaderModuleCreateInfo shader_module_create_info = 
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,              // VkStructureType              sType
			nullptr,                                                  // const void                 * pNext
			0,                                                        // VkShaderModuleCreateFlags    flags
			source_code.size(),                                       // size_t                       codeSize
			reinterpret_cast<uint32_t const *>(source_code.data())    // const uint32_t             * pCode
		};

		VkResult result = vkCreateShaderModule(logical_device, &shader_module_create_info, nullptr, &shader_module);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a shader module." << std::endl;
			return false;
		}
		return true;
	}

}