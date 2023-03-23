//
// Created by Administrator on 2023/3/23.
//
#include "vulkan/vulkan.hpp"
#ifndef VLUKANDEMO_GVKSHADERMODULES_H
#define VLUKANDEMO_GVKSHADERMODULES_H

enum ShaderType
{
    VertexShader,
    FragmentShader,
    ComputeShader
};

class GVKShader {

public:
    GVKShader(ShaderType Type, char *CodeData, uint32_t DataSize);
    void Cleanup();
    ~GVKShader();
private:
    VkShaderModule mShaderModule;
    ShaderType mShaderType;
    VkPipelineShaderStageCreateInfo ShaderStageInfo{};
    VkShaderStageFlagBits GetShaderStage();
};


#endif //VLUKANDEMO_GVKSHADERMODULES_H
