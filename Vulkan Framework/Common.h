#ifndef COMMON_H
#define COMMON_H

#include <iostream>

#if defined _WIN32
	#include <Windows.h>
	#define LIBRARY_TYPE HMODULE
#elif defined __linux
	#define LIBRARY_TYPE void*
#endif

#include "VulkanFunctions.h"


#endif