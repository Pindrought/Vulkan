#pragma once
#include <string>
#include "WindowParameters.h"
class WindowClass
{
public:
	bool Initialize(int width, int height, std::string window_title="Vulkan Window", std::string classname = "VPFWindowClass");
	bool Show();
	bool Hide();
	void Release();
	WindowParameters window_parameters;
	int height = 0;
	int width = 0;
	bool initialized = false;
private:
	std::string classname = "";
};