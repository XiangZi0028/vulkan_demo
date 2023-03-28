//
// Created by Administrator on 2023/3/23.
//
#include "vulkan/vulkan.hpp"
#ifndef VLUKANDEMO_GVKSHADERMODULES_H
#define VLUKANDEMO_GVKSHADERMODULES_H
#include "VulkanGlobalInfo.h"

enum ShaderType
{
    VertexShader,
    FragmentShader,
    ComputeShader
};

class GVKShader {

public:
    GVKShader(ShaderType Type, const std::string FilePath);
    void Cleanup();
    static std::vector<char> ReadFile(const std::string FilePath);
    VkPipelineShaderStageCreateInfo GetShaderStageInfo(){return mShaderStageInfo;}
    ~GVKShader();
private:
    VkShaderModule mShaderModule;
    ShaderType mShaderType;
    VkPipelineShaderStageCreateInfo mShaderStageInfo{};
    VkShaderStageFlagBits GetShaderStage();
};


#endif //VLUKANDEMO_GVKSHADERMODULES_H
