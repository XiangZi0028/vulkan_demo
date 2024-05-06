#include <VulkanRHI/public/VulkanFrameBuffer.h>
#include <VulkanRHI/public/VulkanRenderTarget.h>
#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>

VulkanFrameBuffer::VulkanFrameBuffer(shared_ptr<VulkanDevice> inDevice, TArray(shared_ptr<VulkanRenderTarget>) inRenderTargets)
	: mDevice(inDevice)
	, mRenderTargets(inRenderTargets)
{
	mFrameBufferSize = mRenderTargets[0]->GetImage()->GetImageSize();
};

void VulkanFrameBuffer::InitFrameBuffer(VkRenderPass inRenderPasss)
{
	TArray(VkImageView) attachments;
	for (auto rt : mRenderTargets)
	{
		attachments.push_back(rt->GetImage()->GetImgView()->GetImgView());
	}
	VkFramebufferCreateInfo frameBufferCreateInfo;
	ZeroVulkanStruct(frameBufferCreateInfo, VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO);
	frameBufferCreateInfo.attachmentCount = mRenderTargets.size();
	frameBufferCreateInfo.height = mFrameBufferSize.height;
	frameBufferCreateInfo.width = mFrameBufferSize.width;
	frameBufferCreateInfo.pAttachments = attachments.data();
	frameBufferCreateInfo.renderPass = inRenderPasss;
	frameBufferCreateInfo.flags = 0;
	frameBufferCreateInfo.layers = 1;
	vkCreateFramebuffer(mDevice->GetDevice(), &frameBufferCreateInfo, nullptr, &mFrameBuffer);
};

shared_ptr<VulkanFrameBuffer> VulkanFrameBuffer::Create(shared_ptr<VulkanDevice> inDevice, TArray(shared_ptr<VulkanRenderTarget>) inRenderTargets)
{
	auto CheckRenderTargtes = [&inRenderTargets]()
	{
		if (inRenderTargets.size() == 0)
		{
			return false;
		}
		VkExtent2D imageSize = inRenderTargets[0]->GetImage()->GetImageSize();
		for (auto rt : inRenderTargets)
		{
			VkExtent2D curImageSize = rt->GetImage()->GetImageSize();
			if (curImageSize.width != imageSize.width || curImageSize.height != imageSize.height)
			{
				return false;
			}
		}
		return true;
	};
	if (!CheckRenderTargtes())
	{
		return nullptr;
	}
	shared_ptr<VulkanFrameBuffer> frameBuffer(new VulkanFrameBuffer(inDevice, inRenderTargets));
	frameBuffer->mFrameBufferWidth = inRenderTargets[0]->GetImage()->GetImageSize().width;
	frameBuffer->mFrameBufferHeight = inRenderTargets[0]->GetImage()->GetImageSize().height;

	return frameBuffer;
};


TArray(VkAttachmentDescription) VulkanFrameBuffer::GetAttachmentDescs()
{
	std::vector<VkAttachmentDescription> attachmentDescs;
	for (auto rt : mRenderTargets)
	{
		bool bDepthStencilRT = false;
		attachmentDescs.push_back(rt->GetAttachmentDescription(bDepthStencilRT));
	}
	return attachmentDescs;
}