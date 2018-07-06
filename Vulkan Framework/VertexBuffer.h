#pragma once
#include "Vertex.h"
#include <vector>
#include "LogicalDevice.h"

class CommandPool;

class VertexBuffer
{
public:
	bool Initialize(LogicalDevice & logical_device, CommandPool & command_pool, std::vector<Vertex> vertices, std::vector<uint16_t> indices);
	void Release();
	bool initialized = false;
	VkBuffer vertex_buffer = VK_NULL_HANDLE;
	VkBuffer index_buffer = VK_NULL_HANDLE;
	uint32_t vertex_count = 0;
	uint32_t index_count = 0;
	bool CreateVertexBuffer(LogicalDevice & logical_device, CommandPool & command_pool, std::vector<Vertex> vertices);
	bool CreateIndexBuffer(LogicalDevice & logical_device, CommandPool & command_pool, std::vector<uint16_t> indices);
private:
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	bool CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkDeviceMemory vertex_buffer_memory = VK_NULL_HANDLE;
	VkDeviceMemory index_buffer_memory = VK_NULL_HANDLE;

	VkDevice logical_device = VK_NULL_HANDLE;
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	VkCommandPool command_pool = VK_NULL_HANDLE;
	VkQueue graphics_queue = VK_NULL_HANDLE;

};