#include "UniformBuffer.h"
#include "VulkanFunctions.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

bool UniformBuffer::Initialize(LogicalDevice & logical_device)
{
	Release();
	this->logical_device = logical_device.handle;
	this->physical_device = logical_device.GetPhysicalDeviceHandle();

	VkDeviceSize buffer_size = sizeof(UniformBufferObject);
	if (!CreateBuffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, buffer_memory))
	{
		std::cerr << "Failed to create uniform buffer object." << std::endl;
		return false;
	}

	initialized = true;
	return true;
}

void UniformBuffer::Release()
{
	initialized = false;
	if (buffer != VK_NULL_HANDLE)
	{
		PVF::vkDestroyBuffer(logical_device, buffer, nullptr);
		buffer = VK_NULL_HANDLE;
	}
	if (buffer_memory != VK_NULL_HANDLE)
	{
		PVF::vkFreeMemory(logical_device, buffer_memory, nullptr);
		buffer_memory = VK_NULL_HANDLE;
	}
}

void UniformBuffer::Update(Swapchain & swapchain)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(90.0f), swapchain.extent.width / (float)swapchain.extent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	PVF::vkMapMemory(logical_device, buffer_memory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	PVF::vkUnmapMemory(logical_device, buffer_memory);
}

uint32_t UniformBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

bool UniformBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory)
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
