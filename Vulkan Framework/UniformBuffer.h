#pragma once

#include "Swapchain.h"
#include "glm\glm.hpp"
#include <chrono>

struct UniformBufferObject 
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class UniformBuffer
{
public:
	bool Initialize(LogicalDevice & logical_device);
	void Release();
	void Update(Swapchain & swapchain);
	bool initialized = false;
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
private:
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	bool CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	VkDevice logical_device = VK_NULL_HANDLE;
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
};