#ifndef VULKANDEMO_GVKPIPELINE_H
#define VULKANDEMO_GVKPIPELINE_H
#include "iostream"
#include "vulkan/vulkan.hpp"
using namespace std;
class  GVKPipeline : public enable_shared_from_this<GVKPipeline>
{
    GVKPipeline();
    ~GVKPipeline();
public:

private:
    std::vector<VkDynamicState> mDynamicStates;
    VkPipelineDynamicStateCreateInfo mDynamicStateCreateInfo = {};
    VkPipelineVertexInputStateCreateInfo mVertexInputInfo = {};
    VkPipelineInputAssemblyStateCreateInfo mInputAssmebly = {};
    std::vector<VkRect2D> mScissors{};
    std::vector<VkViewport> mViewports{};
    VkPipelineViewportStateCreateInfo mViewPortInfo{};
    VkPipelineRasterizationStateCreateInfo mRasterInfo{};
    VkPipelineMultisampleStateCreateInfo mMultiSamplingInfo{};
    VkPipelineColorBlendAttachmentState mColorBlendAttachmentInfo{};
    VkPipelineColorBlendStateCreateInfo mColorBlendInfo{};
    //Need To Set
    VkPipelineDepthStencilStateCreateInfo mDepthStencialInfo{};
private:
    void InitDynamicState();
    void InitVertexInputInfo();
    void InitInpuAssmebly();
    void InitViewport();
    void InitScissor();
    void InitViewportInfo();
    void InitRasterInfo();
    void InitMultiSamplingInfo();
    void InitDepthStencialInfo();
    void InitColorBlending();
};

#endif