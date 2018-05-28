#include "Pipeline.h"
#include "VulkanFunctions.h"
#include <iostream>


bool Pipeline::Initialize(LogicalDevice & logical_device, Swapchain & swapchain, VkRenderPass renderpass)
{
	Release();

	this->logical_device = logical_device.handle;

	if (!vertex_shader.Initialize(logical_device.handle, "vert.spv", VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT))
	{
		std::cerr << "Failed to initialize vertex shader." << std::endl;
		return false;
	}

	if (!fragment_shader.Initialize(logical_device.handle, "frag.spv", VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT))
	{
		std::cerr << "Failed to initialize fragment shader." << std::endl;
		return false;
	}

	if (!layout.Initialize(logical_device, swapchain, renderpass))
	{
		std::cerr << "Failed to initialize pipeline layout." << std::endl;
		return false;
	}

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertex_shader.shader_stage_info, fragment_shader.shader_stage_info };

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &layout.vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &layout.inputAssembly;
	pipelineInfo.pViewportState = &layout.viewportState;
	pipelineInfo.pRasterizationState = &layout.rasterizer;
	pipelineInfo.pMultisampleState = &layout.multisampling;
	pipelineInfo.pColorBlendState = &layout.colorBlending;
	pipelineInfo.layout = layout.handle;
	pipelineInfo.renderPass = renderpass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (PVF::vkCreateGraphicsPipelines(logical_device.handle, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &handle) != VK_SUCCESS) 
	{
		std::cerr << "Failed to create graphics pipeline." << std::endl;
		return false;
	}

	vertex_shader.Release();
	fragment_shader.Release();

	initialized = true;
	return true;
}

void Pipeline::Release()
{
	vertex_shader.Release();
	fragment_shader.Release();

	initialized = false;

	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyPipeline(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
	}

	layout.Release();

}
