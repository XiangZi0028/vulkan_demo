//
// Created by Administrator on 2023/3/23.
//
#pragma once
#include "GVKShaderModules.h"
#include <fstream>
GVKShader::GVKShader(ShaderType Type, const std::string FilePath)
:mShaderType(Type)
{
    std::vector<char> CodeData = ReadFile(FilePath);
    VkShaderModuleCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    CreateInfo.codeSize = CodeData.size();
    CreateInfo.pCode = reinterpret_cast<const uint32_t*>(CodeData.data());
    if (vkCreateShaderModule(GVKVariable::GDevice, &CreateInfo, nullptr, &mShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    ShaderStageInfo.stage = GetShaderStage();
    ShaderStageInfo.module = mShaderModule;
    ShaderStageInfo.pName = "main";
}
GVKShader::~GVKShader()
{
}

void GVKShader::Cleanup()
{
    vkDestroyShaderModule(GVKVariable::GDevice, mShaderModule, nullptr);
}

VkShaderStageFlagBits GVKShader::GetShaderStage()
{
    VkShaderStageFlagBits StageFlag;
    switch (mShaderType)
    {
        case ShaderType::VertexShader : StageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT; break;
        case ShaderType::FragmentShader : StageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT; break;
        default: throw std::runtime_error("Unknown ShaderType!");
    }
    return StageFlag;
}

std::vector<char> GVKShader::ReadFile(const std::string FilePath)
{
    std::ifstream file(FilePath, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }
    size_t FileSize = (size_t) file.tellg();
    std::vector<char> buffer(FileSize);
    file.seekg(0);
    file.read(buffer.data(), FileSize);
    file.close();
    return buffer;
}