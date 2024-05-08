#pragma once
#include <vulkan/vulkan.h>
#include <CommonMicro.h>
#include <VulkanCommonDefine.h>

class VulkanDevice;
class VulkanImageView;
class VulkanCommandBuffer;

struct TextureDesc
{
	/*TextureDesc(ETextureDimension inDimension, ETextureCreateFlags inFlags, EPixelFormat inFormat, ClearValueBinding inClearValue, IntPoint2D inExtent, uint16_t inDepth, uint16_t inArraySize, uint8_t inNumMips, uint8_t inNumSamples)
		: mDimension(inDimension)
		, mFlags(inFlags)
		, mFormat(inFormat)
		, mClearValue(inClearValue)
		, mExtent(inExtent)
		, mDepth(inDepth)
		, mArraySize(inArraySize)
		, mNumMips(inNumMips)
		, mNumSamples(inNumSamples)
	{

	};*/

	TextureDesc(ETextureDimension inDimension, ETextureCreateFlags inFlags, EPixelFormat inFormat, ClearValueBinding inClearValue, IntPoint2D inExtent, uint16_t inDepth = 1, uint16_t inArraySize = 1, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
		: mDimension(inDimension)
		, mFlags(inFlags)
		, mFormat(inFormat)
		, mClearValue(inClearValue)
		, mExtent(inExtent)
		, mDepth(inDepth = 1)
		, mArraySize(inArraySize = 1)
		, mNumMips(inNumMips = 1)
		, mNumSamples(inNumSamples = 1)
	{

	};

	//Create 2D TextureDesc
	static TextureDesc Create2D(IntPoint2D inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		const uint16_t arraySize = 1;
		return TextureDesc(ETextureDimension::Tex2D, inFlags, inFormat, inClearValue, inSize, depth, arraySize, inNumMips, inNumSamples);
	};
	//Create 2D Array TextureDesc
	static TextureDesc Create2DArray(IntPoint2D inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint16_t inArraySize, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		return TextureDesc(ETextureDimension::Tex2DArray, inFlags, inFormat, inClearValue, inSize, depth, inArraySize, inNumMips, inNumSamples);
	};

	static TextureDesc Create3D(IntPoint3D inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint8_t inNumMips = 1)
	{
		const uint16_t depth = inSize.z;
		const uint16_t arraySize = 1;
		const uint16_t localNumSamples = 1;
		return TextureDesc(ETextureDimension::Tex3D, inFlags, inFormat, inClearValue, { inSize.x, inSize.y }, depth, arraySize, inNumMips, localNumSamples);
	};

	static TextureDesc CreateCube(int inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		const uint16_t arraySize = 1;
		return TextureDesc(ETextureDimension::TexCube, inFlags, inFormat, inClearValue, { inSize, inSize }, depth, arraySize, inNumMips, inNumSamples);
	};

	static TextureDesc CreateCubeArrat(int inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint16_t inArraySize, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		return TextureDesc(ETextureDimension::TexCubeArray, inFlags, inFormat, inClearValue, { inSize, inSize }, depth, inArraySize, inNumMips, inNumSamples);
	};

	ETextureCreateFlags mFlags = ETextureCreateFlags::TCF_NONE;

	ClearValueBinding mClearValue;

	IntPoint2D mExtent = {1,1};

	uint16_t mDepth = 1;

	uint16_t mArraySize = 1;

	uint8_t mNumMips = 1;

	uint8_t mNumSamples : 5;

	ETextureDimension mDimension : 3;

	EPixelFormat mFormat = EPixelFormat::PF_Unknown;

	EPixelFormat mUAVFormat = EPixelFormat::PF_Unknown;

	bool bForceLieanerTexture = false;

	VkImageCreateInfo mResultVkImageCreateInfo;
};



class VulkanImage : public enable_shared_from_this<VulkanImage>
{
public:
	~VulkanImage();
	static shared_ptr<VulkanImage> CreateTexture(TextureDesc inTexDesc, shared_ptr<VulkanDevice> inDevice);
	
	
	static shared_ptr<VulkanImage> CreateAttachment(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight,
		VkFormat inFormat,
		VkImageUsageFlags inUsage,
		VkImageAspectFlags inImageAspect);

	static shared_ptr<VulkanImage> CreateTexture2D(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight, 
		VkImageUsageFlags inUsage,
		VkFormat inFormat = VK_FORMAT_R8G8B8A8_SRGB,
		uint32_t MipLevels = 1,
		VkImageAspectFlags inImageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);

	static void GenerateImageCreateInfo(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice);

	static uint32_t GetImageLayerCount(VkImageViewType inType, uint32_t inNumArraySlices);

private:
	
	VulkanImage(shared_ptr<VulkanDevice> inDevice, TextureDesc inDesc, VkImage inImage, VkDeviceMemory inVkMemory)
		: mDevice(inDevice)
		, mImg(inImage)
		, mDeviceMemroy(inVkMemory)
		, mTextureDesc(inDesc)
	{

	};
	void InitImage();
	DefineMemberWithGetter(VkImage, Img);
	DefineMemberWithGetter(VkDeviceMemory, DeviceMemroy);
	DefineMemberWithGetter(shared_ptr<VulkanImageView>, ImgView);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithGetter(VkFormat, Format);
	DefineMemberWithGetter(TextureDesc, TextureDesc);

};


class VulkanImageView : public enable_shared_from_this<VulkanImageView>
{
public:
	~VulkanImageView();

	static shared_ptr<VulkanImageView> Create(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
		VkImage inImg,
		VkImageViewType ViewType,
		uint32_t inFirstMip = 0, uint32_t inNumMips = 1,
		uint32_t inArraySliceIndex = 0, uint32_t inNumArraySlices = 1,
		bool bUseIdentitySwizzle = true,
		VkImageUsageFlags inImageUsageFlags = 0);
private:
	
	VulkanImageView(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
		VkImage inImg,
		uint32_t inFirstMip, uint32_t inNumMips,
		uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
		bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags);


	DefineMemberWithGetter(VkImage, Img);
	DefineMemberWithGetter(VkImageView, ImgView);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
};
