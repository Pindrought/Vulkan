#pragma once
#include <string>
#include <vulkan\vulkan.h>
#include <vector>
class Shader
{
public:
	bool Initialize(VkDevice logical_device, const std::string & file_path, VkShaderStageFlagBits shader_stage);
	void Release();
	bool initialized = false;
	VkShaderModule handle = VK_NULL_HANDLE;
	VkPipelineShaderStageCreateInfo shader_stage_info = {};
private:
	bool GetBinaryFileContents(std::string const & filename, std::vector<unsigned char>& contents);
	bool CreateShaderModule(VkDevice logical_device, std::vector<unsigned char> const & source_code);
	VkDevice logical_device = VK_NULL_HANDLE;
};