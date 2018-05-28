#include "VulkanFunctionLoader.h"

namespace PVF
{

	bool LoadFunctionsExportedFromVulkanLoaderLibrary(LIBRARY_TYPE const & vulkan_library)
	{
		#if defined _WIN32
		#define LoadFunction GetProcAddress
		#elif defined __linux
		#define LoadFunction dlsym
		#endif

		#define EXPORTED_VULKAN_FUNCTION( name )                          \
		name = (PFN_##name)LoadFunction( vulkan_library, #name );         \
		if( name == nullptr )                                             \
		{																  \
			std::cerr << "Could not load exported Vulkan function named: "\
			#name << std::endl;                                           \
			return false;                                                 \
		}

		#include "ListOfVulkanFunctions.inl"

		return true;
	}

	bool LoadGlobalLevelFunctions() 
	{
		#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                            \
		name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );				\
		if( name == nullptr )													\
		{																		\
			std::cerr << "Could not load global level Vulkan function named: "  \
			#name << std::endl;													\
			return false;                                                       \
		}

		#include "ListOfVulkanFunctions.inl"
		
		return true;
	}

	bool LoadInstanceLevelFunctions(VkInstance                        instance,
									std::vector<char const *> const & enabled_extensions)
	{
		// Load core Vulkan API instance-level functions
		#define INSTANCE_LEVEL_VULKAN_FUNCTION( name )                              \
		name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );                \
		if( name == nullptr )														\
		{																		    \
		  std::cerr << "Could not load instance-level Vulkan function named: "      \
			#name << std::endl;                                                     \
		  return false;                                                             \
		}

		// Load instance-level functions from enabled extensions
		#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )		\
		for( auto & enabled_extension : enabled_extensions )							\
		{																				\
			if( std::string( enabled_extension ) == std::string( extension ) )			\
			{																			\
				name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );            \
				if( name == nullptr )													\
				{																	    \
					std::cerr << "Could not load instance-level Vulkan function named: "\
					#name << std::endl;													\
					return false;                                                       \
				}																		\
			 }																			\
		}

		#include "ListOfVulkanFunctions.inl"

		return true;
	}

	bool LoadDeviceLevelFunctions(	VkDevice                          logical_device,
									std::vector<char const *> const & enabled_extensions) {
		// Load core Vulkan API device-level functions
		#define DEVICE_LEVEL_VULKAN_FUNCTION( name )                                    \
		name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );			    \
		if( name == nullptr )															\
		{																				\
			std::cerr << "Could not load device-level Vulkan function named: "			\
			#name << std::endl;															\
			return false;																\
		}

		// Load device-level functions from enabled extensions
		#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )      \
		for( auto & enabled_extension : enabled_extensions )						\
		{																			\
		  if( std::string( enabled_extension ) == std::string( extension ) )		\
		  {																			\
			name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );        \
			if( name == nullptr )													\
			{																		\
			  std::cerr << "Could not load device-level Vulkan function named: "    \
				#name << std::endl;                                                 \
			  return false;                                                         \
			}                                                                       \
		  }                                                                         \
		}

#include "ListOfVulkanFunctions.inl"

		return true;
	}

} // namespace VulkanCookbook
