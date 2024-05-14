#pragma once
#include <vulkan/vulkan.h>
#include <CommonMicro.h>
#include <map>
#include <VulkanCommonDefine.h>
#include <iostream>

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


struct ImageViewLayout
{
	bool operator<(const ImageViewLayout& other) const {
		return mHashValue < other.mHashValue;
	}
private:
	uint32_t mHashValue;
};


class VulkanImage : public enable_shared_from_this<VulkanImage>
{
public:
	~VulkanImage();

	static shared_ptr<VulkanImage> CreateTexture(TextureDesc inTexDesc, shared_ptr<VulkanDevice> inDevice);

	static VkImageCreateInfo GenerateImageCreateInfo(TextureDesc inDesc);

	static uint32_t CalcImageLayerCount(VkImageViewType inType, uint32_t inNumArraySlices);

	static uint32_t CalcImageAspectFlagBits(ETextureCreateFlags inTCF);

	VulkanImageView* FindOrCreateImageView(uint32_t inFirstMip = 0, uint32_t inNumMips = 1, uint32_t inArraySliceIndex = 0, uint32_t inNumArraySlices = 1, bool bUseIdentitySwizzle = true);

	VulkanImageView* FindOrCreateImageView(ImageViewLayout& imageLayout);

private:
	
	VulkanImage(shared_ptr<VulkanDevice> inDevice, TextureDesc inDesc, VkImage inImage, VkDeviceMemory inVkMemory)
		: mDevice(inDevice)
		, mImg(inImage)
		, mDeviceMemroy(inVkMemory)
		, mTextureDesc(inDesc)
	{

	};
	typedef std::map<ImageViewLayout, VulkanImageView*> ImageViewMap;
	void InitImage();
	DefineMemberWithGetter(VkImage, Img);
	DefineMemberWithGetter(VkDeviceMemory, DeviceMemroy);
	DefineMemberWithRefGetter(ImageViewMap, ImgViews);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithGetter(VkFormat, Format);
	DefineMemberWithGetter(TextureDesc, TextureDesc);
	ETextureBarrierLayout mCurBarrierLayout = ETextureBarrierLayout::TBL_Undefined;
	//API Param
	uint32_t mImageUsageFlags = 0;

};
struct SubresourceRange;
class VulkanImageView : public enable_shared_from_this<VulkanImageView>
{
public:
	~VulkanImageView();

	static shared_ptr<VulkanImageView> Create(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
		VkImage inImg,
		VkImageUsageFlags inImageUsageFlags,
		uint32_t inFirstMip = 0, uint32_t inNumMips = 1,
		uint32_t inArraySliceIndex = 0, uint32_t inNumArraySlices = 1,
		bool bUseIdentitySwizzle = true);
	static uint8_t CalcImageViewType(ETextureDimension inTexTureDimension);

	static VulkanImageView* CreateNoShared(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
		VkImage inImg,
		VkImageUsageFlags inImageUsageFlags,
		uint32_t inFirstMip = 0, uint32_t inNumMips = 1,
		uint32_t inArraySliceIndex = 0, uint32_t inNumArraySlices = 1,
		bool bUseIdentitySwizzle = true);

private:
	
	VulkanImageView(TextureDesc inDesc, shared_ptr<VulkanDevice> inDevice,
		VkImage inImg,
		uint32_t inFirstMip, uint32_t inNumMips,
		uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
		bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags);

	DefineMemberWithGetter(VkImage, Img)
	DefineMemberWithGetter(VkImageView, ImgView)
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device)
	DefineMemberWithGetter(SubresourceRange,SubresourceRange)

};
