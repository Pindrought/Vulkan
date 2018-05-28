#include "DebugReport.h"
#include "VulkanFunctions.h"

bool checkValidationLayerSupport(const std::vector<const char*> validation_layers) 
{
	uint32_t layerCount;
	PVF::vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	PVF::vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validation_layers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) 
		{
			if (strcmp(layerName, layerProperties.layerName) == 0) 
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) 
		{
			return false;
		}
	}

	return true;
}

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	PFN_vkCreateDebugReportCallbackEXT func = (PFN_vkCreateDebugReportCallbackEXT)PVF::vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) 
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else 
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	PFN_vkDestroyDebugReportCallbackEXT func = (PFN_vkDestroyDebugReportCallbackEXT)PVF::vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) 
	{
		func(instance, callback, pAllocator);
	}
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) 
{
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}

bool DebugReport::Initialize(VkInstance vulkan_instance)
{
	if (enableValidationLayers == false)
		return true;

	Release();

	this->vulkan_instance = vulkan_instance;

	if (!checkValidationLayerSupport(validation_layers))
	{
		std::cerr << "Not all validation layers are supported." << std::endl;
		return false;
	}

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	if (CreateDebugReportCallbackEXT(vulkan_instance, &createInfo, nullptr, &callback) != VK_SUCCESS) 
	{
		std::cerr << "Failed to setup debug report callback." << std::endl;
		return false;
	}

	initialized = true;
	return true;
}

void DebugReport::Release()
{
	initialized = false;
	if (enableValidationLayers) 
	{
		if (callback != VK_NULL_HANDLE)
		{
			DestroyDebugReportCallbackEXT(vulkan_instance, callback, nullptr);
			callback = VK_NULL_HANDLE;
		}
	}
}
