#include "VulkanLibrary.h"
#include "VulkanFunctionLoader.h"


bool VulkanLibrary::Load()
{
	if (initialized)
		return true;
#if defined _WIN32
	handle = LoadLibrary(L"vulkan-1.dll");
#elif defined __linux
	handle = dlopen("libvulkan.so.1", RTLD_NOW);
#endif
	if (handle == nullptr)
		return false;

	if (PVF::LoadFunctionsExportedFromVulkanLoaderLibrary(handle) == false)
	{
		std::cerr << "Failed to load one or more exported functions from vulkan library." << std::endl;
		return false;
	}

	if (PVF::LoadGlobalLevelFunctions() == false)
	{
		std::cerr << "Failed to load one or more global level functions." << std::endl;
		return false;
	}

	initialized = true;
	return true;
}

void VulkanLibrary::Release()
{
	if (handle != nullptr)
	{
		FreeLibrary(handle);
		handle = nullptr;
	}
}
