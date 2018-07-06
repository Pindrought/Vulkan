#pragma once

#include "UniformBuffer.h"

class DescriptorSetLayout
{
public:
	VkDescriptorSetLayout handle = VK_NULL_HANDLE;
	bool Initialize(LogicalDevice & logical_device);
	bool TempInitialize(UniformBuffer & uniform_buffer);
	void Release();
	bool initialized = false;
	//temp
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
private:
	VkDevice logical_device = VK_NULL_HANDLE;
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
};