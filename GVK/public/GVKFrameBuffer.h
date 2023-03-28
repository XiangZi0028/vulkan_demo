//
// Created by Administrator on 2023/3/27.
//
#ifndef VLUKANDEMO_GVKFRAMEBUFFER_H
#define VLUKANDEMO_GVKFRAMEBUFFER_H
#include "vulkan/vulkan.hpp"
#include "iostream"
using namespace std;
class GVKTexture;
class GVKFrameBuffer : public enable_shared_from_this<GVKFrameBuffer>{
public:
    GVKFrameBuffer(uint32_t Width, uint32_t Height);
    ~GVKFrameBuffer();
    void CreateFrameBuffer();
    void SetAttachments(std::vector<GVKTexture*> Attachments);
    void SetRenderPass(VkRenderPass InRenderPass);
    VkFramebuffer GetVKFrameBuffer(){return mFrameBuffer;};
private:
    VkFramebuffer mFrameBuffer;
    VkFramebufferCreateInfo mCreateInfo;
    std::vector<VkImageView> mAttachments;
};


#endif //VLUKANDEMO_GVKFRAMEBUFFER_H
