#pragma once
#include <iostream>
#include <vulkan/vulkan.h>
using namespace std;


enum EVulkanShaderType
{
	VertexShader,
	FragmentShader,
	ComputeShader,
	VulkanShaderTypeMaxNum
};


enum EVulkanCmdBufferPoolType
{
	Graphics,
	Compute,
	Transfer,
	Present
};

enum EVertexIAType
{
	IA_None = 0,
	IA_Position,
	IA_UV0,
	IA_UV1,
	IA_Normal,
	IA_Tangent,
	IA_Color,
	IA_SkinWeight,
	IA_SkinIndex,
	IA_SkinPack,
	IA_InstanceFloat1,
	IA_InstanceFloat2,
	IA_InstanceFloat3,
	IA_InstanceFloat4,
	IA_Custom0,
	IA_Custom1,
	IA_Custom2,
	IA_Custom3,
	IA_Count,
};

enum EPrimitiveTopology
{
	PointList = 0,
	LineList = 1,
	LineStrip = 2,
	TriangleList = 3,
	TriangleStrip = 4,
	TriangleFan = 5,
	LineListAdjacency = 6,
	LineStripAdjacency = 7,
	TriangleListAdjacency = 8,
	TriangleStripAdjacency = 9,
	PatchList = 10,
	PrimitiveTopologyMaxNum
};

enum ECullMode
{
	CM_None, //²»ÌÞ³ý
	CM_CW, //ÌÞ³ýÕýÃæ Ë³Ê±ÕëFront
	CM_CCW, //ÌÞ³ý±³Ãæ Ë³Ê±ÕëFront
};

enum EFrontFace
{
	CounterClockwise,
	Clockwise
};

enum ERasterizerFillMode
{
	Fill,
	Line,
	Point,
	FillRectangleNv,
	PolygonModeMaxNum
};


enum EWritMask
{
	R = 0x01,
	G = 0x02,
	B = 0x04,
	A = 0x08,
	WM_NONE = 0,
	RG = R | G,
	BA = B | A,
	RGB = R | G | B,
	RGBA = R | G | B | A,
};

enum EColorWriteMask
{
	CW_RED = 0x01,
	CW_GREEN = 0x02,
	CW_BLUE = 0x04,
	CW_ALPHA = 0x08,

	CW_NONE = 0,
	CW_RGB = CW_RED | CW_GREEN | CW_BLUE,
	CW_RGBA = CW_RED | CW_GREEN | CW_BLUE | CW_ALPHA,
	CW_RG = CW_RED | CW_GREEN,
	CW_BA = CW_BLUE | CW_ALPHA,

	EColorWriteMask_NumBits = 4,
};


enum EBlendFactor
{
	BF_Zero,
	BF_One,
	BF_SourceColor,
	BF_InverseSourceColor,
	BF_SourceAlpha,
	BF_InverseSourceAlpha,
	BF_DestAlpha,
	BF_InverseDestAlpha,
	BF_DestColor,
	BF_InverseDestColor,
	BF_ConstantBlendFactor,
	BF_InverseConstantBlendFactor,
	BF_Source1Color,
	BF_InverseSource1Color,
	BF_Source1Alpha,
	BF_InverseSource1Alpha,
};

enum EBlendOp
{
	BO_Add,
	BO_Subtract,
	BO_Min,
	BO_Max,
	BO_ReverseSubtract,
	BO_MaxNum
};

enum ECompareOp
{
	Never,
	Less,
	Equal,
	LessOrEqual,
	Greater,
	NotEqual,
	GreaterOrEqual,
	Always,
	CompareOpMaxNum
};

enum EStencilOp
{
	Keep,
	Zero,
	Replace,
	IncrementAndClamp,
	DecrementAndClamp,
	Invert,
	IncrementAndWarp,
	DecrementAndWarp,
	StencilOpMaxNum
};

enum EMultiSampleCount
{
	SampleCount_1,
	SampleCount_2,
	SampleCount_4,
	SampleCount_8,
	SampleCount_16,
	SampleCount_32,
	SampleCount_64,
	SampleCountMaxNum
};


enum EAttachmentType
{
	Color,
	Depth,
	Stencil,
	DepthStencil,
	SwapChain,
	AttachmentTypeMaxNum
};


enum EPixelFormat
{
	PF_Unknown = 0,
	PF_A32B32G32R32F = 1,
	PF_B8G8R8A8 = 2,
	PF_G8 = 3,
	PF_G16 = 4,
	PF_DXT1 = 5,
	PF_DXT3 = 6,
	PF_DXT5 = 7,
	PF_UYVY = 8,
	PF_FloatRGB = 9,
	PF_FloatRGBA = 10,
	PF_DepthStencil = 11,
	PF_ShadowDepth = 12,
	PF_R32_FLOAT = 13,
	PF_G16R16 = 14,
	PF_G16R16F = 15,
	PF_G16R16F_FILTER = 16,
	PF_G32R32F = 17,
	PF_A2B10G10R10 = 18,
	PF_A16B16G16R16 = 19,
	PF_D24 = 20,
	PF_R16F = 21,
	PF_R16F_FILTER = 22,
	PF_BC5 = 23,
	PF_V8U8 = 24,
	PF_A1 = 25,
	PF_FloatR11G11B10 = 26,
	PF_A8 = 27,
	PF_R32_UINT = 28,
	PF_R32_SINT = 29,
	PF_PVRTC2 = 30,
	PF_PVRTC4 = 31,
	PF_R16_UINT = 32,
	PF_R16_SINT = 33,
	PF_R16G16B16A16_UINT = 34,
	PF_R16G16B16A16_SINT = 35,
	PF_R5G6B5_UNORM = 36,
	PF_R8G8B8A8 = 37,
	PF_A8R8G8B8 = 38,
	PF_BC4 = 39,
	PF_R8G8 = 40,
	PF_ATC_RGB = 41,
	PF_ATC_RGBA_E = 42,
	PF_ATC_RGBA_I = 43,
	PF_X24_G8 = 44,
	PF_ETC1 = 45,
	PF_ETC2_RGB = 46,
	PF_ETC2_RGBA = 47,
	PF_R32G32B32A32_UINT = 48,
	PF_R16G16_UINT = 49,
	PF_ASTC_4x4 = 50,
	PF_ASTC_6x6 = 51,
	PF_ASTC_8x8 = 52,
	PF_ASTC_10x10 = 53,
	PF_ASTC_12x12 = 54,
	PF_BC6H = 55,
	PF_BC7 = 56,
	PF_R8_UINT = 57,
	PF_L8 = 58,
	PF_XGXR8 = 59,
	PF_R8G8B8A8_UINT = 60,
	PF_R8G8B8A8_SNORM = 61,
	PF_R16G16B16A16_UNORM = 62,
	PF_R16G16B16A16_SNORM = 63,
	PF_PLATFORM_HDR_0 = 64,
	PF_PLATFORM_HDR_1 = 65,
	PF_PLATFORM_HDR_2 = 66,
	PF_NUMMAX = PF_PLATFORM_HDR_2 + 1,
};

enum class ETextureDimension : uint8_t
{
	Tex1D,
	Tex1DArray,
	Tex2D,
	Tex2DArray,
	Tex3D,
	TexCube,
	TexCubeArray
};

struct PixelFormatInfo
{
	const char* name;
	//int blockSizeX;
	//int blockSizeY;
	//int blockSizeZ;
	//int blockBytes;
	//int numComponents;
	uint32_t platformFormat;
	bool        supported;
	//EPixelFormat pixelFormat;
};


//VkFormat PixelFormatToVKFormat(EPixelFormat inPixelFormat, bool bUseSRGB)
//{
//	VkFormat result = (VkFormat)G_PixelFormats[inPixelFormat].platformFormat;
//}


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


struct ClearValueBinding
{
	struct DepthStencilValue
	{
		float depth;
		uint32_t stencil;
	};

	union ClearValueType
	{
		float color[4];
		DepthStencilValue eepthStencilValue;
	} value;

	EClearBinding colorBinding;
};


enum class ETextureCreateFlags
{
	TCF_NONE = 0,
	TCF_RenderTarget = 1 << 0,
	TCF_ResolveTarget = 1 << 1,
	TCF_DepthStencilRT = 1 << 2,
	TCF_ShaderResource = 1 << 3,
	TCF_SRGB = 1 << 4,
	TCF_CPUWritable = 1 << 5,
	TCF_InputAttachmentRead = 1 << 9,
	TCF_UAV = 1 << 17,
	TCF_Presentable = 1 << 18,
	TCF_DepthStencilResolveTarget = 1 << 27,

};

static const VkImageTiling GVulkanViewTypeTilingMode[7] =
{
	VK_IMAGE_TILING_LINEAR,		// VK_IMAGE_VIEW_TYPE_1D
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_2D
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_3D
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_CUBE
	VK_IMAGE_TILING_LINEAR,		// VK_IMAGE_VIEW_TYPE_1D_ARRAY
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_2D_ARRAY
	VK_IMAGE_TILING_OPTIMAL,	// VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
};


template<typename T>
struct Point2D
{
	Point2D()
	{
		x = 0;
		y = 0;
	};
	Point2D(T inX, T inY)
	{
		x = inX;
		y = inY;
	};
	T x;
	T y;
};

template<typename T>
struct Point3D
{
	Point3D()
	{
		x = 0;
		y = 0;
		z = 0;
	};
	Point3D(T inX, T inY, T inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	};
	T x;
	T y;
	T z;
};
typedef Point2D<int> IntPoint2D;
typedef Point3D<int> IntPoint3D;