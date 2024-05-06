#pragma once
#include <vulkan/vulkan.h>
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>

class VulkanRenderPass;
class VulkanDevice;
class VulkanRenderTarget;
class VulkanFrameBuffer : public enable_shared_from_this<VulkanFrameBuffer>
{
public:
	~VulkanFrameBuffer() {};
	static shared_ptr<VulkanFrameBuffer> Create(shared_ptr<VulkanDevice> inDevice, TArray(shared_ptr<VulkanRenderTarget>) inRenderTargets);
	shared_ptr<VulkanRenderTarget> GetRT(int Index);
	friend class VulkanRenderPass;
private:
	VulkanFrameBuffer(shared_ptr<VulkanDevice> inDevice, TArray(shared_ptr<VulkanRenderTarget>) inRenderTargets);
	void InitFrameBuffer(VkRenderPass inRenderPass);
	TArray(VkAttachmentDescription) GetAttachmentDescs();
	DefineMemberWithGetter(VkExtent2D, FrameBufferSize)
	DefineMemberWithGetter(VkFramebuffer, FrameBuffer)
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device)
	DefineMemberWithGetter(TArray(shared_ptr<VulkanRenderTarget>), RenderTargets)
	DefineMemberDefaultWithGetter(uint32_t, FrameBufferWidth, 0)
	DefineMemberDefaultWithGetter(uint32_t, FrameBufferHeight, 0)
	DefineMemberWithRefGetter(TArray(VkClearValue), ClearValues)
};

