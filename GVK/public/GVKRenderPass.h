//
// Created by Administrator on 2023/3/26.
//

#ifndef VLUKANDEMO_GVKRENDERPASS_H
#define VLUKANDEMO_GVKRENDERPASS_H
#include "vulkan/vulkan.hpp"
#include "GVKSwapChain.h"
#include "GVKPipeline.h"
class  GVKShader;
class GVKRenderPass : public enable_shared_from_this<GVKRenderPass>
{

public:
    GVKRenderPass();
    ~GVKRenderPass();
    void CreateRenderPass();
    void CreateGraphicsRenderPipeline();
    void Cleanup();


    GVKRenderPass* SetPipeline(GVKPipeline *InPipeline);
    GVKRenderPass* SetShaders(GVKShader* VertextShader, GVKShader* FragmentShader, GVKShader* ComputeShader);
private:
    VkAttachmentDescription mColorAttachment{};
    VkAttachmentReference mColorAttachmentRef{};
    VkSubpassDescription mSubPass{};
    VkRenderPass mRenderPass;

private:
    GVKPipeline* mPipeline;
    GVKShader* mVertextShader;
    GVKShader* mFragmentShader;
    GVKShader* mComputeShader;
    std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
private:
    void InitColorAttachment();
    void InitColorAttachmentInfo();
    void InitSubPass();
    void CreateRenderPipeline();
};


#endif //VLUKANDEMO_GVKRENDERPASS_H
