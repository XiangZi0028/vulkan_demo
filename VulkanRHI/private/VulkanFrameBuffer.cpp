#include <VulkanRHI/public/VulkanFrameBuffer.h>
#include <VulkanRHI/public/VulkanRenderTarget.h>
#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>

VulkanFrameBuffer::VulkanFrameBuffer(shared_ptr<VulkanDevice> inDevice, TArray(shared_ptr<VulkanRenderTarget>) inRenderTargets)
	: mDevice(inDevice)
	, mRenderTargets(inRenderTargets)
{
	mFrameBufferSize = mRenderTargets[0]->GetImage()->GetTextureDesc().mExtent;
};

void VulkanFrameBuffer::InitFrameBuffer(VkRenderPass inRenderPasss)
{
	TArray(VkImageView) attachments;
	for (auto rt : mRenderTargets)
	{
		ImageViewLayout ImageViewLayout;
		shared_ptr<VulkanImageView> imgView = rt->GetImage()->FindOrCreateImageView(ImageViewLayout);
		imgView->GetImgView();
	}
	VkFramebufferCreateInfo frameBufferCreateInfo;
	ZeroVulkanStruct(frameBufferCreateInfo, VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO);
	frameBufferCreateInfo.attachmentCount = mRenderTargets.size();
	frameBufferCreateInfo.height = mFrameBufferSize.x;
	frameBufferCreateInfo.width = mFrameBufferSize.y;
	frameBufferCreateInfo.pAttachments = attachments.data();
	frameBufferCreateInfo.renderPass = inRenderPasss;
	frameBufferCreateInfo.flags = 0;
	frameBufferCreateInfo.layers = 1;
	vkCreateFramebuffer(mDevice->GetDevice(), &frameBufferCreateInfo, nullptr, &mFrameBuffer);
};

shared_ptr<VulkanFrameBuffer> VulkanFrameBuffer::Create(shared_ptr<VulkanDevice> inDevice, TArray(shared_ptr<VulkanRenderTarget>) inRenderTargets)
{
	auto CheckRenderTargtes = [&inRenderTargets](IntPoint2D& outimageSize)
	{
		if (inRenderTargets.size() == 0)
		{
			return false;
		}
		outimageSize = inRenderTargets[0]->GetImage()->GetTextureDesc().mExtent;
		for (auto rt : inRenderTargets)
		{
			IntPoint2D curImageSize = rt->GetImage()->GetTextureDesc().mExtent;
			if (curImageSize.x != outimageSize.x || curImageSize.y != outimageSize.y)
			{
				return false;
			}
		}
		return true;
	};
	IntPoint2D outimageSize;
	if (!CheckRenderTargtes(outimageSize))
	{
		return nullptr;
	}
	shared_ptr<VulkanFrameBuffer> frameBuffer(new VulkanFrameBuffer(inDevice, inRenderTargets));
	frameBuffer->mFrameBufferWidth = outimageSize.x;
	frameBuffer->mFrameBufferHeight = outimageSize.y;

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