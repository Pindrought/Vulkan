#include "DescriptorSetLayout.h"
#include "VulkanFunctions.h"

bool DescriptorSetLayout::Initialize(LogicalDevice & logical_device)
{
	Release();
	this->logical_device = logical_device.handle;

	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (PVF::vkCreateDescriptorSetLayout(logical_device.handle, &layoutInfo, nullptr, &handle) != VK_SUCCESS) 
	{
		std::cerr << "Failed to create descriptor set layout." << std::endl;
		return false;
	}

	


	initialized = true;
	return true;
}

bool DescriptorSetLayout::TempInitialize(UniformBuffer & uniform_buffer)
{
	//Temp
	//Create descriptor pool
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = 1;

	if (PVF::vkCreateDescriptorPool(logical_device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		std::cerr << "Failed to create descriptor pool." << std::endl;
		return false;
	}

	//create descriptor set
	VkDescriptorSetLayout layouts[] = { handle };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	if (PVF::vkAllocateDescriptorSets(logical_device, &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		std::cerr << "Failed to allocate descriptor set(s)." << std::endl;
		return false;
	}

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = uniform_buffer.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	PVF::vkUpdateDescriptorSets(logical_device, 1, &descriptorWrite, 0, nullptr);
	//Temp
	return true;
}

void DescriptorSetLayout::Release()
{
	initialized = false;

	//temp
	if (descriptorPool != VK_NULL_HANDLE)
	{
		PVF::vkDestroyDescriptorPool(logical_device, descriptorPool, nullptr);
		descriptorPool = VK_NULL_HANDLE;
	}
	//temp

	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyDescriptorSetLayout(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
	}
}