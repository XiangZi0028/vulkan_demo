#include "GVKPipeline.h"
#include "GVKSwapChain.h"
#include "VulkanGlobalInfo.h"
GVKPipeline::GVKPipeline()
{
    mDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    mDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

    InitDynamicState();
    InitVertexInputInfo();
    InitInpuAssmebly();
    InitScissor();
    InitViewportInfo();
    InitRasterInfo();
    InitMultiSamplingInfo();
    InitDepthStencialInfo();
    InitColorBlendAttachmentInfo();
    InitColorBlending();
    InitGraphicsPipelineCreateInfo();
}

GVKPipeline::~GVKPipeline()
{

}

void GVKPipeline::InitDynamicState()
{

    mDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    mDynamicStateCreateInfo.dynamicStateCount = static_cast<::uint32_t>(mDynamicStates.size());
    mDynamicStateCreateInfo.pDynamicStates = mDynamicStates.data();
    mDynamicStateCreateInfo.pNext = nullptr;
    //mDynamicStateCreateInfo.flags
}

void GVKPipeline::InitInpuAssmebly()
{
    mInputAssmebly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    mInputAssmebly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    mInputAssmebly.primitiveRestartEnable = VK_FALSE;
    mInputAssmebly.pNext = nullptr;
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

void GVKPipeline::InitColorBlendAttachmentInfo()
{
    mColorBlendAttachmentInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    mColorBlendAttachmentInfo.blendEnable = VK_FALSE;
    mColorBlendAttachmentInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    mColorBlendAttachmentInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    mColorBlendAttachmentInfo.colorBlendOp = VK_BLEND_OP_ADD;
    mColorBlendAttachmentInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    mColorBlendAttachmentInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    mColorBlendAttachmentInfo.alphaBlendOp = VK_BLEND_OP_ADD;
}
//关于Blend这块参数的很多东西 需要后面再根据官方的文档看看
void GVKPipeline::InitColorBlending()
{
    mColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    mColorBlendInfo.logicOpEnable = VK_FALSE;
    mColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    mColorBlendInfo.attachmentCount = 1;
    mColorBlendInfo.pAttachments = &mColorBlendAttachmentInfo;
    mColorBlendInfo.blendConstants[0] = 0.0f;
    mColorBlendInfo.blendConstants[1] = 0.0f;
    mColorBlendInfo.blendConstants[2] = 0.0f;
    mColorBlendInfo.blendConstants[3] = 0.0f;
}

void GVKPipeline::CreatePipelineLayout()
{
    //这里这个layout应该是glsl里面的layout(Set,binding)???
    VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
    PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    PipelineLayoutInfo.setLayoutCount = 0;
    PipelineLayoutInfo.pSetLayouts = nullptr;
    PipelineLayoutInfo.pushConstantRangeCount = 0;
    PipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(GVKVariable::GDevice, &PipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void GVKPipeline::Cleanup()
{
    vkDestroyPipeline(GVKVariable::GDevice,mGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(GVKVariable::GDevice, mPipelineLayout, nullptr);
}

void GVKPipeline::InitGraphicsPipelineCreateInfo()
{

    mGraphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    //PipelineInfo.stageCount = static_cast<::uint32_t>(mShaderStages.size());
    //PipelineInfo.pStages = mShaderStages.data();
    mGraphicsPipelineInfo.pVertexInputState = &mVertexInputInfo;
    mGraphicsPipelineInfo.pInputAssemblyState = &mInputAssmebly;
    mGraphicsPipelineInfo.pViewportState = &mViewPortInfo;
    mGraphicsPipelineInfo.pRasterizationState = &mRasterInfo;
    mGraphicsPipelineInfo.pMultisampleState = &mMultiSamplingInfo;
    mGraphicsPipelineInfo.pDepthStencilState = nullptr;
    mGraphicsPipelineInfo.pColorBlendState = &mColorBlendInfo;
    mGraphicsPipelineInfo.pDynamicState = &mDynamicStateCreateInfo;
    mGraphicsPipelineInfo.layout = mPipelineLayout;


    //后期优化扩展性时候可以考虑
    //mGraphicsPipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;//只有当这个启用时才有用
    mGraphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    mGraphicsPipelineInfo.basePipelineIndex -1;

}

GVKPipeline* GVKPipeline::SetShaderStages(std::vector<VkPipelineShaderStageCreateInfo> *InShaderStages)
{
    mGraphicsPipelineInfo.stageCount = static_cast<::uint32_t>(InShaderStages->size());
    mGraphicsPipelineInfo.pStages = InShaderStages->data();
    return this;
}
GVKPipeline* GVKPipeline::SetRenderPass(VkRenderPass RenderPass, ::uint32_t SubpassIndex)
{
    mGraphicsPipelineInfo.renderPass = RenderPass;
    mGraphicsPipelineInfo.subpass = SubpassIndex;
    return this;
}

void GVKPipeline::CreateGraphiPipeline()
{
    if (vkCreateGraphicsPipelines(GVKVariable::GDevice, VK_NULL_HANDLE, 1, &mGraphicsPipelineInfo, nullptr, &mGraphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}


