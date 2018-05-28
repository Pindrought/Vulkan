#ifndef RENDERER_H
#define RENDERER_H
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <vulkan/vk_platform.h>
#include <vector>
#include <iostream>
#include "Common.h"
#include "VulkanLibrary.h"
#include "VulkanInstance.h"
#include "WindowClass.h"
#include "CommandPool.h"
#include "Semaphore.h"
#include "Fence.h"
#include "VertexBuffer.h"

class Renderer
{
private:
	VulkanLibrary vulkan_library;
	DebugReport debug_report;
	VulkanInstance vulkan_instance;
	WindowClass window_class;
	PresentationSurface presentation_surface;
	LogicalDevice logical_device;
	Pipeline pipeline;
	RenderPass renderpass;
	Framebuffers framebuffers;
	CommandPool command_pool;
	PhysicalDevice physical_device;
	VertexBuffer vertex_buffer;

	std::vector<Semaphore> image_available_semaphores;
	std::vector<Semaphore> render_finished_semaphores;
	std::vector<Fence> in_flight_fences;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	size_t currentFrame = 0;

	std::vector<Vertex> vertices;

public:
	bool Initialize(uint32_t width=800, uint32_t height = 600, std::string title = "Vulkan Window");
	bool DrawFrame();
	bool ProcessMessages();
	~Renderer();
	uint32_t width;
	uint32_t height;

	Swapchain swapchain;

};

#endif
