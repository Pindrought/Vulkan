#include "VertexBuffer.h"
#include "VulkanFunctions.h"

uint32_t VertexBuffer::findMemoryType(VkPhysicalDevice physical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	PVF::vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

bool VertexBuffer::Initialize(LogicalDevice & logical_device, std::vector<Vertex> vertices)
{
	Release();
	this->logical_device = logical_device.handle;

	vertex_count = vertices.size();

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (PVF::vkCreateBuffer(logical_device.handle, &bufferInfo, nullptr, &handle) != VK_SUCCESS)
	{
		std::cerr << "Failed to create vertex buffer." << std::endl;
		return false;
	}

	VkMemoryRequirements memRequirements;
	PVF::vkGetBufferMemoryRequirements(logical_device.handle, handle, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(logical_device.GetPhysicalDeviceHandle(), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (PVF::vkAllocateMemory(logical_device.handle, &allocInfo, nullptr, &buffer_memory) != VK_SUCCESS)
	{
		std::cerr << "Failed to allocate vertex buffer memory." << std::endl;
		return false;
	}

	PVF::vkBindBufferMemory(logical_device.handle, handle, buffer_memory, 0);

	void* data;
	PVF::vkMapMemory(logical_device.handle, buffer_memory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	PVF::vkUnmapMemory(logical_device.handle, buffer_memory);
	
	initialized = true;
	return true;
}

void VertexBuffer::Release()
{
	initialized = false;
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyBuffer(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
	}
	if (buffer_memory != VK_NULL_HANDLE)
	{
		PVF::vkFreeMemory(logical_device, buffer_memory, nullptr);
		buffer_memory = VK_NULL_HANDLE;
	}
}
