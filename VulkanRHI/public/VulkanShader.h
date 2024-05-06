#pragma once
#include <vulkan/vulkan.h>
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>
class VulkanDevice;
class VulkanShader : public enable_shared_from_this<VulkanShader>
{

public:
    static shared_ptr<VulkanShader> CreateShader(shared_ptr<VulkanDevice> inDevice, EVulkanShaderType inShaderType, const std::string FilePath, const char* functionEntry = "Main");

    static shared_ptr<VulkanShader> CreateVertexShader(shared_ptr<VulkanDevice> inDevice, const std::string FilePath, const char* functionEntry = "Main");
    static shared_ptr<VulkanShader> CreateComputeShader(shared_ptr<VulkanDevice> inDevice, const std::string FilePath, const char* functionEntry = "Main");
    static shared_ptr<VulkanShader> CreateFragmentShader(shared_ptr<VulkanDevice> inDevice, const std::string FilePath, const char* functionEntry = "Main")
        ;
    ~VulkanShader()
    {

    };
private:
    VulkanShader(shared_ptr<VulkanDevice> inDevice, EVulkanShaderType inType, const std::string FilePath, const char* functionEntry);
    DefineMemberWithGetter(VkShaderModule, ShaderModule);
    DefineMemberWithGetter(VkPipelineShaderStageCreateInfo, ShaderStageInfo);
    EVulkanShaderType mShaderType;
};


