#pragma once
#include "Vertex.h"
#include <vector>
#include "LogicalDevice.h"

class VertexBuffer
{
public:
	bool Initialize(LogicalDevice & logical_device, std::vector<Vertex> vertices);
	void Release();
	bool initialized = false;
	VkBuffer handle = VK_NULL_HANDLE;
	uint32_t vertex_count = 0;
private:
	uint32_t findMemoryType(VkPhysicalDevice physical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
	VkDevice logical_device = VK_NULL_HANDLE;
};