#pragma once
#include<iostream>
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