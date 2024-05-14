#include <VulkanRHI/public/VulkanRenderTarget.h>
#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>

shared_ptr<VulkanRenderTarget> VulkanRenderTarget::CreateAttachment(EAttachmentType inAttachmentType, shared_ptr<VulkanDevice> inDevice, EPixelFormat inFormat, uint32_t inWith, uint32_t inHeight, ClearValueBinding CleaValue)
{
	//VkFormatProperties formatProperties = inDevice->GetPhysicalDeviceFormatProperties(inFormat);
	if (VK_ERROR_FORMAT_NOT_SUPPORTED)
	{

	}
	VkImageUsageFlags attachmentUsage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	VkImageAspectFlags imageAspect = 0;
	uint32_t textureCreateFlags = TCF_ShaderResource;
	switch (inAttachmentType)
	{
		case EAttachmentType::Depth:
		{
			attachmentUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
			textureCreateFlags = textureCreateFlags | TCF_DepthStencilRT;
		};
		case EAttachmentType::Stencil:
		{
			attachmentUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
			textureCreateFlags = textureCreateFlags | TCF_DepthStencilRT;
			break;
		};
		case EAttachmentType::Color:
		{
			attachmentUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_COLOR_BIT;
			textureCreateFlags = textureCreateFlags | TCF_RenderTarget;
			break;
		};
		case EAttachmentType::DepthStencil:
		{
			attachmentUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageAspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
			imageAspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
			textureCreateFlags = textureCreateFlags | TCF_DepthStencilRT;
			break;
		};
		default:
			break;
	};
	TextureDesc rendertarGetDesc = TextureDesc::Create2D(IntPoint2D(inWith, inHeight), inFormat, CleaValue, ETextureCreateFlags(textureCreateFlags));
	shared_ptr<VulkanImage> newImg = VulkanImage::CreateTexture(rendertarGetDesc, inDevice);
		//::CreateAttachment(inDevice, inWith, inHeight, inFormat, attachmentUsage, imageAspect);
	
	shared_ptr<VulkanRenderTarget> newRT(new VulkanRenderTarget(newImg, inAttachmentType));
	return newRT;
	return nullptr;
}

VkAttachmentDescription& VulkanRenderTarget::GetAttachmentDescription(bool& bOutDepthStencil)
{
	VkImageCreateInfo imgInfo = mImage->GenerateImageCreateInfo(mImage->GetTextureDesc());
	mDesc.format = imgInfo.format;
	mDesc.samples = imgInfo.samples;
	
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
		case Color:
		{
			mDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			mDesc.storeOp = VK_ATTACHMENT_STORE_OP_NONE;
			mDesc.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
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


