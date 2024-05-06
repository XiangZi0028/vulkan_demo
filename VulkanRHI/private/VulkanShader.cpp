#include <VulkanRHI/public/VulkanShader.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanRHI/public//FileSystem.h>
#include <vulkan/vulkan.h>

VulkanShader::VulkanShader(shared_ptr<VulkanDevice> inDevice, EVulkanShaderType inShaderType, const std::string FilePath, const char* functionEntry)
{
    std::vector<char> CodeData = FileSys::ReadFile(FilePath);
    VkShaderModuleCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    CreateInfo.codeSize = CodeData.size();
    CreateInfo.pCode = reinterpret_cast<const uint32_t*>(CodeData.data());
    if (vkCreateShaderModule(inDevice->GetDevice(), &CreateInfo, nullptr, &mShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    auto GetShaderStage = [&inShaderType]()
    {
        VkShaderStageFlagBits StageFlag;
        switch (inShaderType)
        {
        case EVulkanShaderType::VertexShader: StageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT; break;
        case EVulkanShaderType::FragmentShader: StageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT; break;
        case EVulkanShaderType::ComputeShader: StageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT; break;
        default: throw std::runtime_error("Unknown ShaderType!");
        }
        return StageFlag;
    };
    ZeroVulkanStruct(mShaderStageInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
    mShaderStageInfo.stage = GetShaderStage();
    mShaderStageInfo.module = mShaderModule;
    mShaderStageInfo.pName = functionEntry;
};

shared_ptr<VulkanShader> VulkanShader::CreateShader(shared_ptr<VulkanDevice> inDevice, EVulkanShaderType inShaderType, const std::string FilePath, const char* functionEntry)
{
    shared_ptr<VulkanShader> newShader(new VulkanShader(inDevice, inShaderType, FilePath, functionEntry));
    return newShader;
};


shared_ptr<VulkanShader> VulkanShader::CreateVertexShader(shared_ptr<VulkanDevice> inDevice, const std::string FilePath, const char* functionEntry)
{
    shared_ptr<VulkanShader> newShader(new VulkanShader(inDevice, EVulkanShaderType::VertexShader, FilePath, functionEntry));
    return newShader;
};

shared_ptr<VulkanShader> VulkanShader::CreateFragmentShader(shared_ptr<VulkanDevice> inDevice, const std::string FilePath, const char* functionEntry)
{
    shared_ptr<VulkanShader> newShader(new VulkanShader(inDevice, EVulkanShaderType::FragmentShader, FilePath, functionEntry));
    return newShader;
};

shared_ptr<VulkanShader> VulkanShader::CreateComputeShader(shared_ptr<VulkanDevice> inDevice, const std::string FilePath, const char* functionEntry)
{
    shared_ptr<VulkanShader> newShader(new VulkanShader(inDevice, EVulkanShaderType::ComputeShader, FilePath, functionEntry));
    return newShader;
};
