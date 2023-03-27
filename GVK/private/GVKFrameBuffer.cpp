//
// Created by Administrator on 2023/3/27.
//
#include "GVKFrameBuffer.h"
#include "GVKTexture.h"
#include "VulkanGlobalInfo.h"

GVKFrameBuffer::GVKFrameBuffer(uint32_t Width, uint32_t Height)
{
    mCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    mCreateInfo.layers = 1;
    mCreateInfo.width = Width;
    mCreateInfo.height = Height;
    mCreateInfo.pNext = nullptr;
}

GVKFrameBuffer::~GVKFrameBuffer()
{

}


void GVKFrameBuffer::CreateFrameBuffer()
{
    if (vkCreateFramebuffer(GVKVariable::GDevice, &mCreateInfo, nullptr, &mFrameBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }
}

void GVKFrameBuffer::SetAttachments(std::vector<GVKTexture*> InAttachments)
{
    std::vector<VkImageView> Attachments;
    for (size_t i = 0; i < InAttachments.size(); i++)
    {
       Attachments.push_back(InAttachments[i]->GetImageView());
    }
    mCreateInfo.attachmentCount = InAttachments.size();
    mCreateInfo.pAttachments = Attachments.data();
}

void GVKFrameBuffer::SetRenderPass(VkRenderPass InRenderPass)
{
    mCreateInfo.renderPass = InRenderPass;
}