#pragma once
#include "Shader.h"
#include "Swapchain.h"
#include "Vertex.h"
#include "DescriptorSetLayout.h"

class PipelineLayout
{
public:
	bool Initialize(LogicalDevice & logical_device, Swapchain & swapchain, VkRenderPass renderpass, std::vector<DescriptorSetLayout> descriptor_set_layouts);
	void Release();
	bool initialized = false;
	VkPipelineLayout handle = VK_NULL_HANDLE;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	VkViewport viewport = {};
	VkRect2D scissor = {};
	VkPipelineViewportStateCreateInfo viewportState = {};
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkPipelineColorBlendStateCreateInfo colorBlending = {};

private:
	VkDevice logical_device = VK_NULL_HANDLE;
	VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = Vertex::getAttributeDescriptions();
};