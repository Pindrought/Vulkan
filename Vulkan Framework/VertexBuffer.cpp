#include "VertexBuffer.h"
#include "VulkanFunctions.h"
#include "CommandPool.h"

bool VertexBuffer::CreateVertexBuffer(LogicalDevice & logical_device, CommandPool & command_pool, std::vector<Vertex> vertices)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	if (!CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory))
	{
		std::cerr << "Failed to create staging buffer." << std::endl;
		return false;
	}

	void* data;
	PVF::vkMapMemory(logical_device.handle, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	PVF::vkUnmapMemory(logical_device.handle, stagingBufferMemory);

	if (!CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer, vertex_buffer_memory))
	{
		std::cerr << "Failed to create vertex buffer." << std::endl;
		return false;
	}

	CopyBuffer(stagingBuffer, vertex_buffer, bufferSize);

	PVF::vkDestroyBuffer(logical_device.handle, stagingBuffer, nullptr);
	PVF::vkFreeMemory(logical_device.handle, stagingBufferMemory, nullptr);

	return true;
}

bool VertexBuffer::CreateIndexBuffer(LogicalDevice & logical_device, CommandPool & command_pool, std::vector<uint16_t> indices)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	if (!CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory))
	{
		std::cerr << "Failed to create staging buffer for index buffer." << std::endl;
		return false;
	}

	void* data;
	PVF::vkMapMemory(logical_device.handle, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	PVF::vkUnmapMemory(logical_device.handle, stagingBufferMemory);

	if (!CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer, index_buffer_memory))
	{
		std::cerr << "Failed to create index buffer." << std::endl;
		return false;
	}

	CopyBuffer(stagingBuffer, index_buffer, bufferSize);

	PVF::vkDestroyBuffer(logical_device.handle, stagingBuffer, nullptr);
	PVF::vkFreeMemory(logical_device.handle, stagingBufferMemory, nullptr);
	return true;
}

uint32_t VertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	PVF::vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

bool VertexBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) 
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (PVF::vkCreateBuffer(logical_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
	{
		std::cerr << "Failed to create buffer." << std::endl;
		return false;
	}

	VkMemoryRequirements memRequirements;
	PVF::vkGetBufferMemoryRequirements(logical_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (PVF::vkAllocateMemory(logical_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
	{
		std::cerr << "Failed to allocate buffer memory." << std::endl;
		return false;
	}

	PVF::vkBindBufferMemory(logical_device, buffer, bufferMemory, 0);
	return true;
}

void VertexBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = command_pool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	PVF::vkAllocateCommandBuffers(logical_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	PVF::vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	PVF::vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	PVF::vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	PVF::vkQueueSubmit(graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
	PVF::vkQueueWaitIdle(graphics_queue);

	PVF::vkFreeCommandBuffers(logical_device, command_pool, 1, &commandBuffer);
}

bool VertexBuffer::Initialize(LogicalDevice & logical_device, CommandPool & command_pool, std::vector<Vertex> vertices, std::vector<uint16_t> indices)
{
	Release();
	this->logical_device = logical_device.handle;
	this->physical_device = logical_device.GetPhysicalDeviceHandle();
	this->command_pool = command_pool.handle;
	this->graphics_queue = command_pool.graphics_queue;
	this->vertex_count = vertices.size();
	this->index_count = indices.size();

	if (!CreateVertexBuffer(logical_device, command_pool, vertices))
	{
		std::cerr << "Failed to create vertex buffer." << std::endl;
		return false;
	}

	if (!CreateIndexBuffer(logical_device, command_pool, indices))
	{
		std::cerr << "Failed to create index buffer." << std::endl;
		return false;
	}
	
	initialized = true;
	return true;
}

void VertexBuffer::Release()
{
	initialized = false;
	if (vertex_buffer != VK_NULL_HANDLE)
	{
		PVF::vkDestroyBuffer(logical_device, vertex_buffer, nullptr);
		vertex_buffer = VK_NULL_HANDLE;
	}
	if (vertex_buffer_memory != VK_NULL_HANDLE)
	{
		PVF::vkFreeMemory(logical_device, vertex_buffer_memory, nullptr);
		vertex_buffer_memory = VK_NULL_HANDLE;
	}
	if (index_buffer != VK_NULL_HANDLE)
	{
		PVF::vkDestroyBuffer(logical_device, index_buffer, nullptr);
		index_buffer = VK_NULL_HANDLE;
	}
	if (index_buffer_memory != VK_NULL_HANDLE)
	{
		PVF::vkFreeMemory(logical_device, index_buffer_memory, nullptr);
		index_buffer_memory = VK_NULL_HANDLE;
	}
}
