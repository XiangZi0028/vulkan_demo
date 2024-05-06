#pragma once
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>

struct VulkanRasterizerStateInitialzer
{
	ERasterizerFillMode FillMode = ERasterizerFillMode::Fill;
	ECullMode CullMode = CM_None;
	float DepthBias = 0.0;
	float SlopScaleDepthBias = 0.0;
	bool bEnableLineAA = false;
	bool bEnableMSAA = true;
};

struct BlendState
{
	bool bEnableBlend = false;
	EWritMask writeMask = EWritMask::RGBA;
	EBlendFactor colorSrcFactor = EBlendFactor::BF_One;
	EBlendFactor colorDstFactor = EBlendFactor::BF_Zero;
	EBlendOp colorOp = EBlendOp::BO_Add;
	EBlendFactor alphaSrcFactor = EBlendFactor::BF_One;
	EBlendFactor alphaDstFactor = EBlendFactor::BF_Zero;
	EBlendOp alphaOp = EBlendOp::BO_Add;
};

struct DepthStencilState
{
	bool bEnableDepthTest = true;
	bool bEnableWriteDepth = true;
	ECompareOp depthCompareOp = ECompareOp::LessOrEqual;
	bool bEnableDepthBoundsTest = false;

	//Back
	bool bEnbaleFrontStencilTest = true;
	EStencilOp stencilFailOp_Back = EStencilOp::Zero;
	EStencilOp stencilPassOp_Back = EStencilOp::Keep;
	EStencilOp depthFailStencilOp_Back = EStencilOp::Keep;
	ECompareOp stencilCompareOp_Back = ECompareOp::Always;
	//Front
	bool bEnbaleBackStencilTest = true;
	EStencilOp stencilFailOp_Front = EStencilOp::Zero;
	EStencilOp stencilPassOp_Front = EStencilOp::Keep;
	EStencilOp depthFailStencilOp_Front = EStencilOp::Keep;
	ECompareOp stencilCompareOp_Front = ECompareOp::Always;
	uint32_t       compareMask;
	uint32_t       writeMask;
};