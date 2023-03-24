#include "GVKPipeline.h"
#include "GVKSwapChain.h"
#include "VulkanGlobalInfo.h"
GVKPipeline::GVKPipeline()
{
    mDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    mDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

}

GVKPipeline::~GVKPipeline()
{

}

void GVKPipeline::InitDynamicState()
{

    mDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    mDynamicStateCreateInfo.dynamicStateCount = static_cast<::uint32_t>(mDynamicStates.size());
    mDynamicStateCreateInfo.pDynamicStates = mDynamicStates.data();
}

void GVKPipeline::InitInpuAssmebly()
{
    mInputAssmebly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    mInputAssmebly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    mInputAssmebly.primitiveRestartEnable = VK_FALSE;
}

void GVKPipeline::InitVertexInputInfo()
{
    mVertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    mVertexInputInfo.vertexBindingDescriptionCount = 0;
    mVertexInputInfo.pVertexBindingDescriptions = nullptr;
    mVertexInputInfo.vertexAttributeDescriptionCount =0;
    mVertexInputInfo.pVertexAttributeDescriptions = nullptr;
}

void GVKPipeline::InitViewport()
{
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) GVKVariable::GGVKSwapChain->GetSwapChainExtent().width;
    viewport.height = (float) GVKVariable::GGVKSwapChain->GetSwapChainExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    mViewports.push_back(viewport);
}
void GVKPipeline::InitScissor() {
    VkRect2D Scissor;
    Scissor.offset = {0, 0};
    Scissor.extent = GVKVariable::GGVKSwapChain->GetSwapChainExtent();
    mScissors.push_back(Scissor);
}

void GVKPipeline::InitRasterInfo()
{
    mRasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    mRasterInfo.depthClampEnable = VK_FALSE;
    mRasterInfo.rasterizerDiscardEnable = VK_FALSE;
    mRasterInfo.polygonMode = VK_POLYGON_MODE_FILL;
    mRasterInfo.lineWidth = 1.0;
    mRasterInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    mRasterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    mRasterInfo.depthBiasEnable = VK_FALSE;
    mRasterInfo.depthBiasConstantFactor = 0.0f; // Optional
    mRasterInfo.depthBiasClamp = 0.0f; // Optional
    mRasterInfo.depthBiasSlopeFactor = 0.0f; // Optional
}

void GVKPipeline::InitViewportInfo()
{
    InitViewport();
    mViewPortInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    mViewPortInfo.viewportCount = static_cast<::uint32_t>(mViewports.size());
    mViewPortInfo.scissorCount = static_cast<::uint32_t>(mScissors.size());
    mViewPortInfo.pViewports  = mViewports.data();
    mViewPortInfo.pScissors = mScissors.data();
}

//启用多重采样的话需要开启GPU特性
void GVKPipeline::InitMultiSamplingInfo()
{
    mMultiSamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    mMultiSamplingInfo.sampleShadingEnable = VK_FALSE;
    mMultiSamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    mMultiSamplingInfo.minSampleShading = 1.0f;
    mMultiSamplingInfo.pSampleMask = nullptr;
    mMultiSamplingInfo.alphaToCoverageEnable = VK_FALSE;
    mMultiSamplingInfo.alphaToOneEnable = VK_FALSE;
}

void GVKPipeline::InitDepthStencialInfo()
{

}

void GVKPipeline::InitColorBlending()
{

}
