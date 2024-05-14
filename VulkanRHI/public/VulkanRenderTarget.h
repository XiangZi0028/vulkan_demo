#pragma once
#include <vulkan/vulkan.h>
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>



class VulkanImage;
class VulkanDevice;
class VulkanRenderTarget : public enable_shared_from_this<VulkanRenderTarget>
{
public:
	~VulkanRenderTarget() {};

	//static shared_ptr<VulkanRenderTarget> CreateRenderTarget(shared_ptr<VulkanDevice> inDevice, VkFormat inFormat, uint32_t inWith, uint32_t inHeight, VkImageUsageFlags inUsage);

	static shared_ptr<VulkanRenderTarget> VulkanRenderTarget::CreateAttachment(EAttachmentType inAttachmentType, shared_ptr<VulkanDevice> inDevice, EPixelFormat inFormat, uint32_t inWith, uint32_t inHeight, ClearValueBinding CleaValue = {});

	VkAttachmentDescription& GetAttachmentDescription(bool& bOutDepthStencil);

private:
	VulkanRenderTarget(shared_ptr<VulkanImage> inImg, EAttachmentType inAttachmentType)
		: mImage(inImg)
		, mAttachemntType(inAttachmentType)
	{};
	
	static bool CheckAttachmentTypeAndFormat(EAttachmentType inAttachmentType, VkFormat inFormat);

	std::string mName;

	VkAttachmentDescription mDesc;
	EAttachmentType mAttachemntType;
	DefineMemberDefaultWithGetter(shared_ptr<VulkanImage>, Image, nullptr);
};



