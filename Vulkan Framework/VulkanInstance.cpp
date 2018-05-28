#include "VulkanInstance.h"
#include "VulkanFunctionLoader.h"

bool VulkanInstance::Initialize(std::vector<const char*> extensions, std::string application_name, DebugReport & debug_report)
{
	//Add necessary extensions
	extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
	#ifdef VK_USE_PLATFORM_WIN32_KHR
		extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	#elif defined VK_USE_PLATFORM_XCB_KHR
		extensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
	#elif defined VK_USE_PLATFORM_XLIB_KHR
		extensions.emplace_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
	#endif

	if (!CreateVulkanInstanceWithWsiExtensionsEnabled(extensions, application_name.c_str(), debug_report.validation_layers))
	{
		std::cerr << "Failed to create vulkan instance." << std::endl;
		return false;
	}

	if (!PVF::LoadInstanceLevelFunctions(handle, extensions))
	{
		std::cerr << "Failed to load instance level vulkan functions." << std::endl;
		return false;
	}

	return true;
}

void VulkanInstance::Release()
{
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyInstance(handle, nullptr);
		handle = VK_NULL_HANDLE;
		initialized = false;
	}
}

bool VulkanInstance::CreateVulkanInstanceWithWsiExtensionsEnabled(std::vector<char const*>& desired_extensions, char const * const application_name, const std::vector<const char*> validation_layers)
{
	std::vector<VkExtensionProperties> available_extensions;
	if (!GetAvailableInstanceExtensions(available_extensions))
	{
		std::cerr << "Failed to Get Available Instance Extensions." << std::endl;
		return false;
	}

	if (enableValidationLayers) 
	{
		desired_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	for (auto & extension : desired_extensions)
	{
		if (!IsExtensionSupported(available_extensions, extension))
		{
			std::cerr << "Extension named '" << extension << "' is not supported by an Instance object." << std::endl;
			return false;
		}
	}

	VkApplicationInfo application_info =
	{
		VK_STRUCTURE_TYPE_APPLICATION_INFO,                 // VkStructureType           sType
		nullptr,                                            // const void              * pNext
		application_name,                                   // const char              * pApplicationName
		VK_MAKE_VERSION(1, 0, 0),							// uint32_t                  applicationVersion
		"PVF Vulkan Engine",                                // const char              * pEngineName
		VK_MAKE_VERSION(1, 0, 0),							// uint32_t                  engineVersion
		VK_MAKE_VERSION(1, 0, 0)							// uint32_t                  apiVersion
	};

	VkInstanceCreateInfo instance_create_info =
	{
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,             // VkStructureType           sType
		nullptr,                                            // const void              * pNext
		0,                                                  // VkInstanceCreateFlags     flags
		&application_info,                                  // const VkApplicationInfo * pApplicationInfo
		0,                                                  // uint32_t                  enabledLayerCount
		nullptr,                                            // const char * const      * ppEnabledLayerNames
		static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                  enabledExtensionCount
		desired_extensions.data()                           // const char * const      * ppEnabledExtensionNames
	};

	if (enableValidationLayers && validation_layers.size()>0)
	{
		instance_create_info.enabledLayerCount = validation_layers.size();
		instance_create_info.ppEnabledLayerNames = validation_layers.data();
	}

	VkResult result = PVF::vkCreateInstance(&instance_create_info, nullptr, &handle);
	if ((result != VK_SUCCESS) || (handle == VK_NULL_HANDLE))
	{
		std::cerr << "Could not create Vulkan instance." << std::endl;
		return false;
	}

	return true;

	return false;
}

bool VulkanInstance::GetAvailableInstanceExtensions(std::vector<VkExtensionProperties>& available_extensions)
{
	uint32_t extensions_count = 0;
	VkResult result = VK_SUCCESS;

	result = PVF::vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr);
	if ((result != VK_SUCCESS) || (extensions_count == 0))
	{
		std::cerr << "Could not get the number of instance extensions." << std::endl;
		return false;
	}

	available_extensions.resize(extensions_count);
	result = PVF::vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, available_extensions.data());
	if ((result != VK_SUCCESS) || (extensions_count == 0))
	{
		std::cerr << "Could not enumerate instance extensions." << std::endl;
		return false;
	}

	return true;
}

bool VulkanInstance::IsExtensionSupported(std::vector<VkExtensionProperties> const & available_extensions, char const * const extension)
{
	for (auto & available_extension : available_extensions)
	{
		if (strstr(available_extension.extensionName, extension))
		{
			return true;
		}
	}
	return false;
}
