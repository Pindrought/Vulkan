#include "Shader.h"
#include <iostream>
#include <fstream>
#include "VulkanFunctions.h"

bool Shader::Initialize(VkDevice logical_device, const std::string & file_path, VkShaderStageFlagBits shader_stage)
{
	this->logical_device = logical_device;
	std::vector<unsigned char> vertex_shader_data;

	if (!GetBinaryFileContents(file_path, vertex_shader_data))
	{
		std::cerr << "Failed to get binary file contents of vertex shader for file: " << file_path << std::endl;
		return false;
	}

	if (!CreateShaderModule(logical_device, vertex_shader_data))
	{
		std::cerr << "Failed to create vertex shader module for file: " << file_path << std::endl;
		return false;
	}

	shader_stage_info = {};
	shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_info.stage = shader_stage;
	shader_stage_info.module = handle;
	shader_stage_info.pName = "main"; //We will just assume every shader's entry point is main

	initialized = true;
	return true;
}

void Shader::Release()
{
	if (handle != VK_NULL_HANDLE)
	{
		PVF::vkDestroyShaderModule(logical_device, handle, nullptr);
		handle = VK_NULL_HANDLE;
		initialized = false;
	}
}

bool Shader::GetBinaryFileContents(std::string const & filename, std::vector<unsigned char>& contents)
{
	contents.clear();

	std::ifstream file(filename, std::ios::binary);
	if (file.fail())
	{
		std::cout << "Could not open '" << filename << "' file." << std::endl;
		return false;
	}

	std::streampos begin;
	std::streampos end;
	begin = file.tellg();
	file.seekg(0, std::ios::end);
	end = file.tellg();

	if ((end - begin) == 0)
	{
		std::cout << "The '" << filename << "' file is empty." << std::endl;
		return false;
	}
	contents.resize(static_cast<size_t>(end - begin));
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(contents.data()), end - begin);
	file.close();

	return true;
}

bool Shader::CreateShaderModule(VkDevice logical_device, std::vector<unsigned char> const & source_code)
{
	VkShaderModuleCreateInfo shader_module_create_info =
	{
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,              // VkStructureType              sType
		nullptr,                                                  // const void                 * pNext
		0,                                                        // VkShaderModuleCreateFlags    flags
		source_code.size(),                                       // size_t                       codeSize
		reinterpret_cast<uint32_t const *>(source_code.data())    // const uint32_t             * pCode
	};

	VkResult result = PVF::vkCreateShaderModule(logical_device, &shader_module_create_info, nullptr, &handle);
	if (VK_SUCCESS != result)
	{
		std::cout << "Could not create a shader module." << std::endl;
		return false;
	}
	return true;
}
