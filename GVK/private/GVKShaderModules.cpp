//
// Created by Administrator on 2023/3/23.
//

#include "GVKShaderModules.h"
#include "VulkanGlobalInfo.h"
GVKShader::GVKShader(ShaderType Type, char *CodeData, uint32_t DataSize)
:mShaderType(Type)
{
    VkShaderModuleCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    CreateInfo.codeSize = DataSize;
    CreateInfo.pCode = reinterpret_cast<const uint32_t*>(CodeData);
    if (vkCreateShaderModule(GVK::GDevice, &CreateInfo, nullptr, &mShaderModule) != VK_SUCCESS) {
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
    vkDestroyShaderModule(GVK::GDevice, mShaderModule, nullptr);
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