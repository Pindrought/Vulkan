#pragma once
#include "vulkan\vulkan.h"

struct QueueParameters
{
	VkQueue   handle = VK_NULL_HANDLE;
	uint32_t  family_index = UINT_MAX;
};