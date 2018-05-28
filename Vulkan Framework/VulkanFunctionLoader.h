#ifndef VULKANFUNCTIONSLOADER_H
#define VULKANFUNCTIONSLOADER_H

#include "Common.h"

namespace PVF 
{

	bool LoadFunctionsExportedFromVulkanLoaderLibrary(LIBRARY_TYPE const & vulkan_library);

	bool LoadGlobalLevelFunctions();

	bool LoadInstanceLevelFunctions(VkInstance                        instance,
									std::vector<char const *> const & enabled_extensions);

	bool LoadDeviceLevelFunctions(	VkDevice                          logical_device,
									std::vector<char const *> const & enabled_extensions);
}

#endif // VULKANFUNCTIONSLOADER_H