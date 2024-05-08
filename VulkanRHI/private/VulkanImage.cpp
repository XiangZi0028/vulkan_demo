#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <vulkan/vulkan.h>
//Vulkan Image
VulkanImage::~VulkanImage()
{
	if (mImgView)
	{
		mImgView.reset();
	}
	if (mImg != VK_NULL_HANDLE)
	{
		vkDestroyImage(mDevice->GetDevice(), mImg, nullptr);
	}
	if (mDeviceMemroy != VK_NULL_HANDLE)
	{
		vkFreeMemory(mDevice->GetDevice(), mDeviceMemroy, nullptr);
	}
};
uint32_t VulkanImage::GetImageLayerCount(VkImageViewType inType, uint32_t inNumArraySlices)
{
	switch (inType)
	{
	case VK_IMAGE_VIEW_TYPE_3D:
		return 1;
		break;
	case VK_IMAGE_VIEW_TYPE_CUBE:
		if (inNumArraySlices != 1)
		{
			std::runtime_error("Error! Cube can only has one slice!");
		}
		return 6;
		break;
	case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
		return 6 * inNumArraySlices;
		break;
	case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
	case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
		return inNumArraySlices;
		break;
	default:
		return 1;
		break;
	}
}

void VulkanImage::InitImage()
{
	mImgView = VulkanImageView::Create(mTextureDesc, mDevice, mImg);
};

shared_ptr<VulkanImage> VulkanImage::CreateTexture2D(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight, VkImageUsageFlags inUsage, VkFormat inFormat, uint32_t inMipLevels, VkImageAspectFlags inImageAspect,  VkSampleCountFlagBits inSampleCount)
{
	//Create Image
	VkImageCreateInfo imgCreateInfo;
	ZeroVulkanStruct(imgCreateInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
	imgCreateInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
	imgCreateInfo.extent.width = inWidth;
	imgCreateInfo.extent.height = inHeight;
	imgCreateInfo.extent.depth = 1;
	imgCreateInfo.format = inFormat;
	imgCreateInfo.arrayLayers = 1;
	imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgCreateInfo.samples = inSampleCount;
	imgCreateInfo.mipLevels = inMipLevels;
	imgCreateInfo.usage = inUsage;

	VkImage newImage = VK_NULL_HANDLE;
	vkCreateImage(inDevice->GetDevice(), &imgCreateInfo, nullptr, &newImage);
	//Allocated Memory And Bind Image And Memory 
	VkMemoryRequirements imgMemoryRequirement;
	vkGetImageMemoryRequirements(inDevice->GetDevice(), newImage, &imgMemoryRequirement);
	VkMemoryAllocateInfo memAllocInfo;
	ZeroVulkanStruct(memAllocInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
	memAllocInfo.allocationSize = imgMemoryRequirement.size;
	VkPhysicalDeviceMemoryProperties gpuMemoryProperties;
	// The returned structure contains memoryTypes and memoryHeaps members
	vkGetPhysicalDeviceMemoryProperties(inDevice->GetGpu(), &gpuMemoryProperties);
	// Find available memory type Index
	int memoryTypeIndex = -1;
	for (uint32_t Index = 0; Index < gpuMemoryProperties.memoryTypeCount; Index++) {
		if ((imgMemoryRequirement.memoryTypeBits & (1 << Index)) && (gpuMemoryProperties.memoryTypes[Index].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			memoryTypeIndex = Index;
			break;
		}
	}
	if (memoryTypeIndex == -1)
	{
		throw std::runtime_error("failed to find suitable memory type!");
	}
	VkDeviceMemory newMemory;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;
	vkAllocateMemory(inDevice->GetDevice(), &memAllocInfo, nullptr, &newMemory);
	vkBindImageMemory(inDevice->GetDevice(), newImage, newMemory, 0);
	shared_ptr<VulkanImageView> newImageView = VulkanImageView::Create(inDevice, newImage, VK_IMAGE_VIEW_TYPE_2D, inImageAspect, inFormat, 0,  inMipLevels, 0, 1 );
	shared_ptr<VulkanImage> newVulkanImage(new VulkanImage(inDevice, newImageView, newImage, newMemory));
	newVulkanImage->InitImage(inWidth, inHeight, inFormat);
	return  newVulkanImage;
}
void VulkanImage::GenerateImageCreateInfo(TextureDesc inTexDesc, shared_ptr<VulkanDevice> inDevice)
{
	VkImageTiling outImageTilling = VK_IMAGE_TILING_LINEAR;
	VkImageUsageFlags outImageUsage = (uint32_t)0;
	outImageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	outImageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	outImageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
	outImageTilling = inTexDesc.bForceLieanerTexture ? VK_IMAGE_TILING_LINEAR : GVulkanViewTypeTilingMode[7];
	//VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	switch (inTexDesc.mFlags)
	{
		case ETextureCreateFlags::TCF_NONE:
			break;
		case ETextureCreateFlags::TCF_Presentable:
		{
			outImageUsage |= VK_IMAGE_USAGE_STORAGE_BIT;
			break;
		};
		case ETextureCreateFlags::TCF_RenderTarget:
		case ETextureCreateFlags::TCF_DepthStencilRT:
		{
			if (EnumHasAnyFlags(inTexDesc.mFlags , ETextureCreateFlags::TCF_InputAttachmentRead))
			{
				outImageUsage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
			}
			outImageUsage |= EnumHasAnyFlags(inTexDesc.mFlags, ETextureCreateFlags::TCF_RenderTarget) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			outImageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			break;
		};
		case ETextureCreateFlags::TCF_ResolveTarget:
		{
			outImageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				break;
		};
		case ETextureCreateFlags::TCF_DepthStencilResolveTarget:
		{
			outImageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				break;
		};
		case ETextureCreateFlags::TCF_ShaderResource:
		{
			outImageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
			break;
		};
		case ETextureCreateFlags::TCF_SRGB:
			break;
		case ETextureCreateFlags::TCF_CPUWritable:
		{
			outImageUsage |= VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;
			break;
		};
		case ETextureCreateFlags::TCF_UAV:
		{
			break;
			outImageUsage |=  VK_IMAGE_USAGE_STORAGE_BIT;
		};
		default:
			break;
	}

	VkImageCreateInfo imgCreateInfo;
	ZeroVulkanStruct(imgCreateInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
	imgCreateInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
	imgCreateInfo.extent.width = inTexDesc.mExtent.x;
	imgCreateInfo.extent.height = inTexDesc.mExtent.y;
	imgCreateInfo.extent.depth = inTexDesc.mDepth;
	imgCreateInfo.format; //todo //PF to format inTexDesc.mFormat
	imgCreateInfo.arrayLayers = inTexDesc.mArraySize;
	imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imgCreateInfo.tiling = outImageTilling;
	imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgCreateInfo.samples = VkSampleCountFlagBits(1 << inTexDesc.mNumSamples);
	imgCreateInfo.mipLevels = inTexDesc.mNumMips;
	imgCreateInfo.usage = outImageUsage;

	VkImage newImage = VK_NULL_HANDLE;
	vkCreateImage(inDevice->GetDevice(), &imgCreateInfo, nullptr, &newImage);
	//Allocated Memory And Bind Image And Memory 
	VkMemoryRequirements imgMemoryRequirement;
	vkGetImageMemoryRequirements(inDevice->GetDevice(), newImage, &imgMemoryRequirement);
	VkMemoryAllocateInfo memAllocInfo;
	ZeroVulkanStruct(memAllocInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
	memAllocInfo.allocationSize = imgMemoryRequirement.size;
	VkPhysicalDeviceMemoryProperties gpuMemoryProperties;
	// The returned structure contains memoryTypes and memoryHeaps members
	vkGetPhysicalDeviceMemoryProperties(inDevice->GetGpu(), &gpuMemoryProperties);
	// Find available memory type Index
	int memoryTypeIndex = -1;
	for (uint32_t Index = 0; Index < gpuMemoryProperties.memoryTypeCount; Index++) {
		if ((imgMemoryRequirement.memoryTypeBits & (1 << Index)) && (gpuMemoryProperties.memoryTypes[Index].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			memoryTypeIndex = Index;
			break;
		}
	}
	if (memoryTypeIndex == -1)
	{
		throw std::runtime_error("failed to find suitable memory type!");
	}
	VkDeviceMemory newMemory;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;
	vkAllocateMemory(inDevice->GetDevice(), &memAllocInfo, nullptr, &newMemory);
	vkBindImageMemory(inDevice->GetDevice(), newImage, newMemory, 0);
	shared_ptr<VulkanImage> newVulkanImage(new VulkanImage(inDevice, newImageView, newImage, newMemory));
	newVulkanImage->InitImage();
	return  newVulkanImage;
}

shared_ptr<VulkanImage> VulkanImage::CreateTexture(TextureDesc inTexDesc, shared_ptr<VulkanDevice> inDevice)
{
	return nullptr;
}

shared_ptr<VulkanImage> VulkanImage::CreateAttachment(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight,
	VkFormat inFormat,
	VkImageUsageFlags inUsage,
	VkImageAspectFlags inImageAspect)
{
	shared_ptr<VulkanImage> newAttachment = CreateTexture2D(inDevice, inWidth, inHeight, inUsage, inFormat, 1, inImageAspect);
	return newAttachment;
}


//Vulkan Image View

VulkanImageView::~VulkanImageView()
{
	if (mImgView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(mDevice->GetDevice(), mImgView, nullptr);
	}
};
shared_ptr<VulkanImageView> VulkanImageView::Create(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
	VkImage inImg, VkImageViewType inViewType,
	uint32_t inFirstMip, uint32_t inNumMips,
	uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
	bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags)
{
	shared_ptr<VulkanImageView> newImageView(new VulkanImageView(inDevice, inImg, inViewType, inFirstMip, inNumMips, inArraySliceIndex, inNumArraySlices, bUseIdentitySwizzle, inImageUsageFlags));
	return newImageView;
}

VulkanImageView::VulkanImageView(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
	VkImage inImg,
	uint32_t inFirstMip, uint32_t inNumMips,
	uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
	bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags)
	:mDevice(inDevice)
{
	auto GetImageViewType = [&inDesc, &inNumArraySlices]()
	{
		switch (inDesc.mDimension)
		{
		case ETextureDimension::Tex1D:
		case ETextureDimension::Tex2D:
		{
			return inNumArraySlices;
			break;
		}
		case ETextureDimension::Tex1DArray:
		case ETextureDimension::Tex2DArray:
		{
			break;
		}
		default:
			break;
		}
	};

	VkImageViewCreateInfo imageViewCreateInfo;
	ZeroVulkanStruct(imageViewCreateInfo, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
	imageViewCreateInfo.image = inImg;
	imageViewCreateInfo.subresourceRange.baseMipLevel = inFirstMip;
	imageViewCreateInfo.subresourceRange.levelCount = inNumMips;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = inArraySliceIndex;
	imageViewCreateInfo.subresourceRange.layerCount = VulkanImage::GetImageLayerCount(inViewType, inNumArraySlices);
	imageViewCreateInfo.subresourceRange.aspectMask = inAspectFlag;
	imageViewCreateInfo.viewType = inViewType;
	imageViewCreateInfo.format = inDesc.mResultVkImageCreateInfo.format;
	if (bUseIdentitySwizzle)
	{
		imageViewCreateInfo.components.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
	}
	else
	{

	}
	//imageViewCreateInfo.flags = 
	vkCreateImageView(inDevice->GetDevice(), &imageViewCreateInfo, nullptr, &mImgView);
}
