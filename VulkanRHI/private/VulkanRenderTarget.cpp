#include <VulkanRHI/public/VulkanRenderTarget.h>
#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>

shared_ptr<VulkanRenderTarget> VulkanRenderTarget::CreateAttachment(EAttachmentType inAttachmentType, shared_ptr<VulkanDevice> inDevice, VkFormat inFormat, uint32_t inWith, uint32_t inHeight)
{
	VkFormatProperties formatProperties = inDevice->GetPhysicalDeviceFormatProperties(inFormat);
	if (VK_ERROR_FORMAT_NOT_SUPPORTED)
	{

	}
	VkImageUsageFlags attachmentUsage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	VkImageAspectFlags imageAspect = 0;
	switch (inAttachmentType)
	{
		case EAttachmentType::Depth:
		{
			attachmentUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
			break;
		};
		case EAttachmentType::Stencil:
		{
			attachmentUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
			break;
		};
		case EAttachmentType::Color:
		{
			attachmentUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_COLOR_BIT;
			break;
		};
		case EAttachmentType::DepthStencil:
		{
			attachmentUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
			imageAspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
			break;
		};
		case EAttachmentType::SwapChain:
		{
			attachmentUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_COLOR_BIT;

			break;
		}
		default:
			break;
	};
	shared_ptr<VulkanImage> newImg= VulkanImage::CreateAttachment(inDevice, inWith, inHeight, inFormat, attachmentUsage, imageAspect);
	
	shared_ptr<VulkanRenderTarget> newRT(new VulkanRenderTarget(newImg, inAttachmentType));
	return newRT;
}

VkAttachmentDescription& VulkanRenderTarget::GetAttachmentDescription(bool& bOutDepthStencil)
{
	mDesc.format = mImage->GetFormat();
	mDesc.samples = VK_SAMPLE_COUNT_1_BIT;
	mDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	mDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	mDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	mDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	mDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	mDesc.finalLayout = VK_IMAGE_LAYOUT_GENERAL;
	mDesc.flags = 0;
	bOutDepthStencil = false;
	switch (mAttachemntType)
	{
		case Depth || Stencil || DepthStencil:
		{
			bOutDepthStencil = true;
			mDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			mDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
			mDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			break;
		}
		case SwapChain:
		{
			mDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			mDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			mDesc.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			break;
		}
		case Color:
		{
			mDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			mDesc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			mDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			break;
		}

	};
	
	return mDesc;
}

bool VulkanRenderTarget::CheckAttachmentTypeAndFormat(EAttachmentType inAttachmentType, VkFormat inFormat)
{
	switch (inAttachmentType)
	{
		case EAttachmentType::Depth:
		{

			break;
		};
		case EAttachmentType::Stencil:
		{
			break;
		};
		case EAttachmentType::Color:
		{

			break;
		};
		case EAttachmentType::DepthStencil:
		{
			break;
		};
	}
	return true;
}


