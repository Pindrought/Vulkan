#pragma once
#include "DebugReport.h"
#include <vector>
#include <string> 

class VulkanInstance
{
public:
	bool Initialize(std::vector<const char*> extensions, std::string application_name, DebugReport & debug_report);
	void Release();
	VkInstance handle = VK_NULL_HANDLE;
	bool initialized = false;
private:
	bool CreateVulkanInstanceWithWsiExtensionsEnabled(std::vector<char const *> & desired_extensions, char const * const application_name, const std::vector<const char*> validation_layers);
	bool GetAvailableInstanceExtensions(std::vector<VkExtensionProperties> & available_extensions);
	bool IsExtensionSupported(std::vector<VkExtensionProperties> const & available_extensions, char const * const extension);
};