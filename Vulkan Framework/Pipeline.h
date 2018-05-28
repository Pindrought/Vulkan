#pragma once
#include "PipelineLayout.h"
class Pipeline
{
public:
	bool Initialize(LogicalDevice & logical_device, Swapchain & swapchain, VkRenderPass renderpass);
	void Release();
	bool initialized = false;
	VkPipeline handle = VK_NULL_HANDLE;
	PipelineLayout layout;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
	Shader vertex_shader;
	Shader fragment_shader;
};