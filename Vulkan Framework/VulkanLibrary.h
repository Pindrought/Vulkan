#pragma once

#if defined _WIN32
#include <Windows.h>
#define LIBRARY_TYPE HMODULE
#elif defined __linux
#define LIBRARY_TYPE void*
#endif

class VulkanLibrary
{
public:
	bool Load();
	void Release();
	bool initialized = false;
	LIBRARY_TYPE handle = nullptr;
};