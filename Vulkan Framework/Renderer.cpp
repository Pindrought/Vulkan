#include "Renderer.h"
#include "VulkanFunctionLoader.h"
#include <assert.h> //for assert
#include <cstdlib> //for std::exit
#include "PhysicalDeviceFinder.h"
#include <algorithm>
#include <set>
#undef max
#include <limits>

bool Renderer::Initialize(uint32_t width, uint32_t height, std::string title)
{
	this->width = width;
	this->height = height;

	vertices =
	{
		{ {-0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f },{ 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } }
	};



	if (!vulkan_library.Load())
	{
		std::cerr << "Failed to load vulkan library." << std::endl;
		return false;
	}

	if (!vulkan_instance.Initialize({}, "Application Name", debug_report))
	{
		std::cerr << "Failed to Initialize Vulkan Instance." << std::endl;
		return false;
	}

	if (!debug_report.Initialize(vulkan_instance.handle))
	{
		std::cerr << "Failed to initialize debug report callback." << std::endl;
		return false;
	}

	if (!window_class.Initialize(width, height, title))
	{
		std::cerr << "Failed to initialize window class." << std::endl;
		return false;
	}

	if (!presentation_surface.Initialize(vulkan_instance.handle, window_class.window_parameters))
	{
		std::cerr << "Failed to initialize presentation surface." << std::endl;
		return false;
	}

	VkPhysicalDeviceFeatures required_features = {};
	required_features.geometryShader = VK_TRUE;

	PhysicalDeviceFinder physical_device_finder(vulkan_instance.handle);
	std::vector<PhysicalDevice> physical_devices = physical_device_finder.GetAllPhysicalDevicesWithSupportedFeaturesAndPresentationToSurface(required_features, true, true, presentation_surface.handle);

	if (physical_devices.size() == 0)
	{
		std::cerr << "No physical devices have the required features." << std::endl;
		return false;
	}

	std::vector<char const *> logical_device_extensions;
	logical_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	for (auto & physical_device : physical_devices)
	{
		if (logical_device.Initialize(physical_device, logical_device_extensions, required_features))
		{
			this->physical_device = physical_device;
			break;
		}
	}

	if (!logical_device.initialized)
	{
		std::cerr << "Failed to initialize logical device." << std::endl;
		return false;
	}
	
	if (!logical_device.WaitForAllSubmittedCommandsToFinish())
	{
		std::cerr << "Failed to wait for submitted commands to finish." << std::endl;
		return false;
	}

	if (!swapchain.Initialize(logical_device, presentation_surface, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, { width,height }))
	{
		std::cerr << "Failed to initialized swapchain." << std::endl;
		return false;
	}

	if (!renderpass.Initialize(logical_device, swapchain))
	{
		std::cerr << "Failed to initialize render pass." << std::endl;
		return false;
	}

	if (!pipeline.Initialize(logical_device, swapchain, renderpass.handle))
	{
		std::cerr << "Failed to initialize pipeline." << std::endl;
		return false;
	}

	if (!framebuffers.Initialize(swapchain, renderpass, logical_device))
	{
		std::cerr << "Failed to initialize frame buffers." << std::endl;
		return false;
	}

	if (!vertex_buffer.Initialize(logical_device, vertices))
	{
		std::cerr << "Failed to initialize vertex buffer." << std::endl;
		return false;
	}

	if (!command_pool.Initialize(physical_device, logical_device, swapchain, pipeline, framebuffers, renderpass, vertex_buffer))
	{
		std::cerr << "Failed to initialize command pool." << std::endl;
		return false;
	}

	image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

	for (auto & semaphore : image_available_semaphores)
	{
		if (!semaphore.Initialize(logical_device))
		{
			std::cerr << "Failed to create image available semaphore." << std::endl;
			return false;
		}
	}

	for (auto & semaphore : render_finished_semaphores)
	{
		if (!semaphore.Initialize(logical_device))
		{
			std::cerr << "Failed to create render finished semaphore." << std::endl;
			return false;
		}
	}

	for (auto & fence : in_flight_fences)
	{
		if (!fence.Initialize(logical_device.handle))
		{
			std::cerr << "Failed to create fence." << std::endl;
			return false;
		}
	}

	window_class.Show();

	return true;
}

bool Renderer::DrawFrame()
{
	if (!in_flight_fences[currentFrame].WaitForFence(std::numeric_limits<uint64_t>::max()))
	{
		std::cerr << "Failed to wait for fence from drawframe. Exiting..." << std::endl;
		return false;
	}
	if (!in_flight_fences[currentFrame].ResetFence())
	{
		std::cerr << "Failed to reset fence from drawframe. Exiting..." << std::endl;
		return false;
	}

	uint32_t imageIndex;
	PVF::vkAcquireNextImageKHR(logical_device.handle, swapchain.handle, std::numeric_limits<uint64_t>::max(), image_available_semaphores[currentFrame].handle, VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { image_available_semaphores[currentFrame].handle };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command_pool.command_buffers.buffers[imageIndex];

	VkSemaphore signalSemaphores[] = { render_finished_semaphores[currentFrame].handle };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (PVF::vkQueueSubmit(physical_device.graphics_queue.handle, 1, &submitInfo, in_flight_fences[currentFrame].handle) != VK_SUCCESS) 
	{
		std::cerr << "Failed to submit draw command to buffer." << std::endl;
		return false;
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain.handle };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	PVF::vkQueuePresentKHR(physical_device.presentation_queue.handle, &presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	return true;
}

bool Renderer::ProcessMessages()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	if (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		window_class.window_parameters.hWnd, //Handle to window we are checking messages for
		0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
		0,    //Maximum Filter Msg Value
		PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
	{
		return false;
	}
	return true;
}

Renderer::~Renderer()
{
	logical_device.WaitForAllSubmittedCommandsToFinish();

	vertex_buffer.Release();

	for (auto & fence : in_flight_fences)
	{
		fence.Release();
	}
	in_flight_fences.clear();

	for (auto & semaphore : render_finished_semaphores)
	{
		semaphore.Release();
	}
	render_finished_semaphores.clear();

	for (auto & semaphore : image_available_semaphores)
	{
		semaphore.Release();
	}
	image_available_semaphores.clear();

	command_pool.Release();

	framebuffers.Release();

	pipeline.Release();

	renderpass.Release();

	swapchain.Release();

	logical_device.Release();

	presentation_surface.Release();

	debug_report.Release();

	vulkan_instance.Release();

	vulkan_library.Release();

	window_class.Release();
}
