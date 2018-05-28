#pragma once
#include <cstdint>
#include <vector>

struct QueueInfo
{
	uint32_t           family_index;
	std::vector<float> priorities;
};