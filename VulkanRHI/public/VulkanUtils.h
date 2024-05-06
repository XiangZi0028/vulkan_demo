#pragma once
#include "CommonMicro.h"
#include "VulkanCommonDefine.h"
template<typename T>
struct Point2D
{
	T x;
	T y;
};

template<typename T>
struct Point3D
{
	T x;
	T y;
	T z;
};
typedef IntPoint2D Point2D<int>;
typedef IntPoint3D Point3D<int>;


class PipelineBarrier : public enable_shared_from_this<PipelineBarrier>
{

};

enum class EResourceAccess
{
	// Used when the previous state of a resource is not known,
	// which implies we have to flush all GPU caches etc.
	Unknown = 0,

	// Read states
	CPURead = 1 << 0,
	Present = 1 << 1,
	IndirectArgs = 1 << 2,
	VertexOrIndexBuffer = 1 << 3,
	SRVCompute = 1 << 4,
	SRVGraphics = 1 << 5,
	CopySrc = 1 << 6,
	ResolveSrc = 1 << 7,
	DSVRead = 1 << 8,

	// Read-write states
	UAVCompute = 1 << 9,
	UAVGraphics = 1 << 10,
	RTV = 1 << 11,
	CopyDest = 1 << 12,
	ResolveDst = 1 << 13,
	DSVWrite = 1 << 14,

	// Ray tracing acceleration structure states.
	// Buffer that contains an AS must always be in either of these states.
	// BVHRead -- required for AS inputs to build/update/copy/trace commands.
	// BVHWrite -- required for AS outputs of build/update/copy commands.
	BVHRead = 1 << 15,
	BVHWrite = 1 << 16,

	// Invalid released state (transient resources)
	Discard = 1 << 17,

	// Shading Rate Source
	ShadingRateSource = 1 << 18,

	Last = ShadingRateSource,
	None = Unknown,
	Mask = (Last << 1) - 1,

	// A mask of the two possible SRV states
	SRVMask = SRVCompute | SRVGraphics,

	// A mask of the two possible UAV states
	UAVMask = UAVCompute | UAVGraphics,

	// A mask of all bits representing read-only states which cannot be combined with other write states.
	ReadOnlyExclusiveMask = CPURead | Present | IndirectArgs | VertexOrIndexBuffer | SRVGraphics | SRVCompute | CopySrc | ResolveSrc | BVHRead,

	// A mask of all bits representing read-only states on the compute pipe which cannot be combined with other write states.
	ReadOnlyExclusiveComputeMask = CPURead | IndirectArgs | SRVCompute | CopySrc | BVHRead,

	// A mask of all bits representing read-only states which may be combined with other write states.
	ReadOnlyMask = ReadOnlyExclusiveMask | DSVRead | ShadingRateSource,

	// A mask of all bits representing readable states which may also include writable states.
	ReadableMask = ReadOnlyMask | UAVMask,

	// A mask of all bits representing write-only states which cannot be combined with other read states.
	WriteOnlyExclusiveMask = RTV | CopyDest | ResolveDst,

	// A mask of all bits representing write-only states which may be combined with other read states.
	WriteOnlyMask = WriteOnlyExclusiveMask | DSVWrite,

	// A mask of all bits representing writable states which may also include readable states.
	WritableMask = WriteOnlyMask | UAVMask | BVHWrite
};

enum class EClearBinding
{
	ENoneBound, //no clear color associated with this target.  Target will not do hardware clears on most platforms
	EColorBound, //target has a clear color bound.  Clears will use the bound color, and do hardware clears.
	EDepthStencilBound, //target has a depthstencil value bound.  Clears will use the bound values and do hardware clears.
};

enum class ETextureCreateFlags
{
	TCF_NONE = 0,
	TCF_RenderTarget = 1ull << 0,
	TCF_ResolveTarget = 1ull << 1,
	TCF_DepthStencilRT = 1ull << 2,
	TCF_ShaderResource = 1ull << 3,
	TCF_SRGB = 1ull << 4,
	TCF_CPUWritable = 1ull << 5,
	TCF_InputAttachmentRead = 1ull << 9,
	TCF_UAV = 1ull << 17,
	TCF_Presentable = 1ull << 18,
	TCF_DepthStencilResolveTarget = 1ull << 27,

};
static const VkImageTiling GVulkanViewTypeTilingMode[VK_IMAGE_VIEW_TYPE_RANGE_SIZE] =
{
	VK_IMAGE_TILING_LINEAR,		// VK_IMAGE_VIEW_TYPE_1D
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_2D
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_3D
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_CUBE
	VK_IMAGE_TILING_LINEAR,		// VK_IMAGE_VIEW_TYPE_1D_ARRAY
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_2D_ARRAY
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
};

 GetUsageFormTextureCreateFlag(ETextureCreateFlags inTextureCreateFlag, VkImageUsageFlagBits& outImageUsage, VkImageTiling& outImageTilling, bool bForceLieanerTexture = false)
{
	VkImageUsageFlagBits outImageUsage = 0;
	outImageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	outImageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	outImageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
	outImageTilling = bForceLieanerTexture ? VK_IMAGE_TILING_LINEAR : GVulkanViewTypeTilingMode[];
	//VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	switch (inTextureCreateFlag)
	{
	case ETextureCreateFlags::TCF_NONE:
		break;
	case ETextureCreateFlags::TCF_Presentable:
	{
		outImageUsage |= VK_IMAGE_USAGE_STORAGE_BIT;
		break;
	};
	case ETextureCreateFlags::TCF_RenderTarget || ETextureCreateFlags::TCF_DepthStencilRT:
	{
		if (inTextureCreateFlag & ETextureCreateFlags::TCF_InputAttachmentRead)
		{
			outImageUsage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}
		outImageUsage |= (inTextureCreateFlag & ETextureCreateFlags::TCF_RenderTarget) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			outImageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		break;
	};
	case ETextureCreateFlags::TCF_ResolveTarget:
	{
		outImageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
		break;
	};
	case ETextureCreateFlags::TCF_ResolveTarget:
	{
		outImageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
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
		outImageUsage |= VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT
		break;
	}
	case ETextureCreateFlags::TCF_UAV:
	{
		break;
		outImageUsage |= VK_IMAGE_USAGE_STORAGE_BIT
	}
		default:
			break;
	}
}
enum class EPixelFormat
{
	PF_Unknown = 0,
	PF_A32B32G32R32F = 1,
	PF_B8G8R8A8 = 2,
	PF_R8 = 3,
	PF_R16 = 4,
	PF_RGB16F = 9, 
	PF_RGBA16F = 10, 
	PF_DepthStencil = 11,
	PF_ShadowDepth = 12,
};

enum class ETextureDimension : uint8_t
{
	Tex2D,
	Tex2DArray,
	Tex3D,
	TexCube,
	TexCubeArray
};

struct ClearValueBinding
{
	struct DepthStencilValue
	{
		float depth;
		uint32 stencil;
	};

	union ClearValueType
	{
		float color[4];
		DepthStencilValue eepthStencilValue;
	} value;

	EClearBinding colorBinding;
};


struct TextureDesc
{
	TextureDesc(ETextureDimension inDimension, ETextureCreateFlags inFlags, EPixelFormat inFormat, ClearValueBinding inClearValue, IntPoint2D inExtent, uint16_t inDepth, uint16_t inArraySize, uint8_t inNumMips, uint8_t inNumSamples)
		: mDimension(inDimension)
		, mFlags(inFlags)
		, mFormat(inFormat)
		, mClearValue(inClearValue)
		, mExtent(inExtent)
		, mDepth(inDepth)
		, mArraySzie(inArraySize)
		, mNumMips(inNumMips)
		, mNumSamples(inNumSamples)
	{

	};

	TextureDesc(ETextureDimension inDimension, ETextureCreateFlags inFlags, EPixelFormat inFormat, ClearValueBinding inClearValue, IntPoint2D inExtent, uint16_t inDepth = 1, uint16_t inArraySize = 1, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
		: mDimension(inDimension)
		, mFlags(inFlags)
		, mFormat(inFormat)
		, mClearValue(inClearValue)
		, mExtent(inExtent)
		, mDepth(inDepth)
		, mArraySzie(inArraySize)
		, mNumMips(inNumMips)
		, mNumSamples(inNumSamples)
	{

	};

	//Create 2D TextureDesc
	static Create2D(IntPoint2D inSize,  EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		const uint16_t arraySize = 1;
		return TextureDesc(ETextureDimension::Tex2D, inFlags, inFormat, inClearValue, inSize, depth, arraySize, inNumMips, inNumSamples);
	};
	//Create 2D Array TextureDesc
	static Create2DArray(IntPoint2D inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint16_t inArraySize, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		return TextureDesc(ETextureDimension::Tex2DArray, inFlags, inFormat, inClearValue, inSize, depth, inArraySize, inNumMips, inNumSamples);
	};

	static Create3D(IntPoint3D inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint8_t inNumMips = 1)
	{
		const uint16_t depth = inSize.z;
		const uint16_t arraySize = 1;
		const uint16_t localNumSamples = 1;
		return TextureDesc(ETextureDimension::Tex3D, inFlags, inFormat, inClearValue, { inSize.x, inSize.y }, depth, arraySize, inNumMips, localNumSamples);
	};

	static CreateCube(int inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		const uint16_t arraySize = 1;
		return TextureDesc(ETextureDimension::TexCube, inFlags, inFormat, inClearValue, { inSize, inSize }, depth, arraySize, inNumMips, inNumSamples);
	};

	static CreateCubeArrat(int inSize, EPixelFormat inFormat, ClearValueBinding inClearValue, ETextureCreateFlags inFlags, uint16_t inArraySize, uint8_t inNumMips = 1, uint8_t inNumSamples = 1)
	{
		const uint16_t depth = 1;
		return TextureDesc(ETextureDimension:::TexCubeArray, inFlags, inFormat, inClearValue, { inSize, inSize }, depth, inArraySize, inNumMips, inNumSamples);
	};


	/** Texture flags passed on to RHI texture. */
	ETextureCreateFlags mFlags = TC_NONE;

	/** Clear value to use when fast-clearing the texture. */
	ClearValueBinding mClearValue;

	/** Extent of the texture in x and y. */
	IntPoint2D mExtent = IntPoint2D(1, 1);

	/** Depth of the texture if the dimension is 3D. */
	uint16_t mDepth = 1;

	/** The number of array elements in the texture. (Keep at 1 if dimension is 3D). */
	uint16_t mArraySize = 1;

	/** Number of mips in the texture mip-map chain. */
	uint8_t mNumMips = 1;

	/** Number of samples in the texture. >1 for MSAA. */
	uint8_t mNumSamples : 5;

	/** Texture dimension to use when creating the RHI texture. */
	ETextureDimension mDimension : 3;

	/** Pixel format used to create RHI texture. */
	EPixelFormat mFormat = PF_Unknown;

	/** Texture format used when creating the UAV. PF_Unknown means to use the default one (same as Format). */
	EPixelFormat mUAVFormat = PF_Unknown;

};

namespace VulkanUtils
{
	
}