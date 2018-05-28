#ifndef VULKANFUNCTIONS_H
#define VULKANFUNCTIONS_H

#include "CustomStructs.h"
#include <iostream>
#include <fstream>
#include <string>

namespace PVF
{
	#define EXPORTED_VULKAN_FUNCTION( name ) extern PFN_##name name;
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) extern PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) extern PFN_##name name;

	#include "ListOfVulkanFunctions.inl"

	bool CreateVulkanInstance(	std::vector<char const *> const & desired_extensions,
								char const * const                application_name,
								VkInstance                      & instance);

	bool EnumerateAvailablePhysicalDevices(	VkInstance                      instance,
											std::vector<VkPhysicalDevice> & available_devices);

	bool CheckAvailableDeviceExtensions(VkPhysicalDevice                     physical_device,
										std::vector<VkExtensionProperties> & available_extensions);

	void GetFeaturesAndPropertiesOfPhysicalDevice(	VkPhysicalDevice             physical_device,
													VkPhysicalDeviceFeatures   & device_features,
													VkPhysicalDeviceProperties & device_properties);

	bool CheckAvailableQueueFamiliesAndTheirProperties(	VkPhysicalDevice                       physical_device,
														std::vector<VkQueueFamilyProperties> & queue_families);

	bool SelectIndexOfQueueFamilyWithDesiredCapabilities(	VkPhysicalDevice   physical_device,
															VkQueueFlags       desired_capabilities,
															uint32_t         & queue_family_index);

	bool CreateLogicalDevice(	VkPhysicalDevice                  physical_device,
								std::vector<QueueInfo>            queue_infos,
								std::vector<char const *> const & desired_extensions,
								VkPhysicalDeviceFeatures        * desired_features,
								VkDevice                        & logical_device);

	bool CreateLogicalDeviceWithWsiExtensionsEnabled(	VkPhysicalDevice            physical_device,
														std::vector< QueueInfo >    queue_infos,
														std::vector<char const *> & desired_extensions,
														VkPhysicalDeviceFeatures  * desired_features,
														VkDevice                  & logical_device);

	bool CreatePresentationSurface(	VkInstance         instance,
									WindowParameters   window_parameters,
									VkSurfaceKHR     & presentation_surface);

	bool SelectQueueFamilyThatSupportsPresentationToGivenSurface(	VkPhysicalDevice   physical_device,
																	VkSurfaceKHR       presentation_surface,
																	uint32_t         & queue_family_index);

	bool SelectDesiredPresentationMode(	VkPhysicalDevice   physical_device,
										VkSurfaceKHR       presentation_surface,
										VkPresentModeKHR   desired_present_mode,
										VkPresentModeKHR & present_mode);

	bool GetCapabilitiesOfPresentationSurface(	VkPhysicalDevice           physical_device,
												VkSurfaceKHR               presentation_surface,
												VkSurfaceCapabilitiesKHR & surface_capabilities);

	bool SelectNumberOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
										uint32_t                       & number_of_images);

	bool ChooseSizeOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
										VkExtent2D                     & size_of_images);

	bool SelectDesiredUsageScenariosOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
														VkImageUsageFlags                desired_usages,
														VkImageUsageFlags              & image_usage);

	bool SelectTransformationOfSwapchainImages(	VkSurfaceCapabilitiesKHR const & surface_capabilities,
												VkSurfaceTransformFlagBitsKHR    desired_transform,
												VkSurfaceTransformFlagBitsKHR  & surface_transform);

	bool SelectFormatOfSwapchainImages(	VkPhysicalDevice     physical_device,
										VkSurfaceKHR         presentation_surface,
										VkSurfaceFormatKHR   desired_surface_format,
										VkFormat           & image_format,
										VkColorSpaceKHR    & image_color_space);

	bool CreateSwapchain(	VkDevice                        logical_device,
							VkSurfaceKHR                    presentation_surface,
							uint32_t                        image_count,
							VkSurfaceFormatKHR              surface_format,
							VkExtent2D                      image_size,
							VkImageUsageFlags               image_usage,
							VkSurfaceTransformFlagBitsKHR   surface_transform,
							VkPresentModeKHR                present_mode,
							VkSwapchainKHR                & old_swapchain,
							VkSwapchainKHR                & swapchain);

	bool GetHandlesOfSwapchainImages(	VkDevice               logical_device,
										VkSwapchainKHR         swapchain,
										std::vector<VkImage> & swapchain_images);

	bool CreateSwapchainWithR8G8B8A8FormatAndMailboxPresentMode(VkPhysicalDevice       physical_device,
																VkSurfaceKHR           presentation_surface,
																VkDevice               logical_device,
																VkImageUsageFlags      swapchain_image_usage,
																VkExtent2D           & image_size,
																VkFormat             & image_format,
																VkSwapchainKHR       & old_swapchain,
																VkSwapchainKHR       & swapchain,
																std::vector<VkImage> & swapchain_images);

	bool CreateCommandPool(	VkDevice                   logical_device,
							VkCommandPoolCreateFlags   parameters,
							uint32_t                   queue_family,
							VkCommandPool            & command_pool);

	bool AllocateCommandBuffers(VkDevice                       logical_device,
								VkCommandPool                  command_pool,
								VkCommandBufferLevel           level,
								uint32_t                       count,
								std::vector<VkCommandBuffer> & command_buffers);

	bool BeginCommandBufferRecordingOperation(	VkCommandBuffer                  command_buffer,
												VkCommandBufferUsageFlags        usage,
												VkCommandBufferInheritanceInfo * secondary_command_buffer_info);

	bool EndCommandBufferRecordingOperation(VkCommandBuffer command_buffer);

	bool CreateSemaphore(	VkDevice      logical_device,
							VkSemaphore & semaphore);

	bool CreateFence(	VkDevice   logical_device,
						bool       signaled,
						VkFence  & fence);

	bool WaitForFences(	VkDevice                     logical_device,
						std::vector<VkFence> const & fences,
						VkBool32                     wait_for_all,
						uint64_t                     timeout);

	bool ResetFences(	VkDevice                     logical_device,
						std::vector<VkFence> const & fences);

	bool SubmitCommandBuffersToQueue(	VkQueue                         queue,
										std::vector<WaitSemaphoreInfo>  wait_semaphore_infos,
										std::vector<VkCommandBuffer>    command_buffers,
										std::vector<VkSemaphore>        signal_semaphores,
										VkFence                         fence);

	bool WaitForAllSubmittedCommandsToBeFinished(VkDevice logical_device);

	bool CreateImageView(	VkDevice             logical_device,
							VkImage              image,
							VkImageViewType      view_type,
							VkFormat             format,
							VkImageAspectFlags   aspect,
							VkImageView        & image_view);

	bool GetBinaryFileContents(	std::string const          & filename,
								std::vector<unsigned char> & contents);

	bool CreateShaderModule(VkDevice                           logical_device,
							std::vector<unsigned char> const & source_code,
							VkShaderModule                   & shader_module);

}

#endif