//
// Created by Administrator on 2023/3/26.
//

#include "GVKRenderPass.h"
#include "VulkanGlobalInfo.h"
#include "GVKShaderModules.h"
GVKRenderPass::GVKRenderPass()
{
    InitColorAttachment();
    InitColorAttachmentInfo();
    InitSubPass();
}

GVKRenderPass::~GVKRenderPass()
{

}

void GVKRenderPass::InitColorAttachment()
{
    mColorAttachment.format = GVKVariable::GGVKSwapChain->GetSwapChainImgFormat();
    mColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    mColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    mColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    mColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    mColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

}
void GVKRenderPass::InitColorAttachmentInfo()
{
    mColorAttachmentRef.attachment = 0; //Ë÷Òý
    mColorAttachmentRef.layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
}

void GVKRenderPass::InitSubPass()
{
    mSubPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    mSubPass.colorAttachmentCount = 1;
    mSubPass.pColorAttachments = &mColorAttachmentRef;
}

void GVKRenderPass::CreateRenderPass()
{
    VkRenderPassCreateInfo RenderPassCreateInfo{};
    RenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    RenderPassCreateInfo.attachmentCount = 1;
    RenderPassCreateInfo.pAttachments = &mColorAttachment;
    RenderPassCreateInfo.subpassCount = 1;
    RenderPassCreateInfo.pSubpasses = &mSubPass;

    if(vkCreateRenderPass(GVKVariable::GDevice,&RenderPassCreateInfo, nullptr,&mRenderPass))
    {
        throw std::runtime_error("Faild to create RenderPass!");
    }
}

void GVKRenderPass::Cleanup()
{
    if(mPipeline != nullptr)
    {
        mPipeline->Cleanup();
    }
    vkDestroyRenderPass(GVKVariable::GDevice,mRenderPass, nullptr);
}

GVKRenderPass *GVKRenderPass::SetPipeline(GVKPipeline* InPipeline)
{
    mPipeline = InPipeline;
    return this;
}

GVKRenderPass* GVKRenderPass::SetShaders(GVKShader* VertextShader, GVKShader* FragmentShader, GVKShader* ComputeShader)
{
    if (VertextShader != nullptr)
    {
        mVertextShader = VertextShader;
        mShaderStages.push_back(mVertextShader->GetShaderStageInfo());
    }
    if (FragmentShader != nullptr)
    {
        mFragmentShader = FragmentShader;
        mShaderStages.push_back(FragmentShader->GetShaderStageInfo());

    }
    if (ComputeShader != nullptr)
    {
        mComputeShader = ComputeShader;
        mShaderStages.push_back(ComputeShader->GetShaderStageInfo());

    }
    return this;
}

void GVKRenderPass::CreateGraphicsRenderPipeline()
{
    mPipeline->SetShaderStages(&mShaderStages)
                ->SetRenderPass(mRenderPass,0);
    mPipeline->CreateGraphiPipeline();
}