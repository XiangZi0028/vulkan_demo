#pragma once
#include <vulkan/vulkan.h>
#include <VulkanCommonDefine.h>
#include <VulkanStats.h>
#include <CommonMicro.h>
#include <map>
#include <iostream>
#include <optional>
extern std::vector<char*> PipelineCachedData;
using namespace std;
#define MaxSimultaneousRenderTargets 8
class VulkanShader;
class VulkanDevice;
class VulkanRenderPass;
class PipelineState
{
public:
	PipelineState()
	{

	};
	~PipelineState()
	{

	};
	friend class VulkanPipeline;
	EPrimitiveTopology mPrimitiveType = EPrimitiveTopology::TriangleList;
	VulkanRasterizerStateInitialzer mRasterizerStateInitialzer;
	DepthStencilState mDepthAndStencilStateInitialzer;
	BlendState mBlendStateInitialzers[MaxSimultaneousRenderTargets];
	bool bDepthBounds = false;
	//有待理解
	//struct MultiSampleState
	//{
	//	bool bEnableSampleShading = false;
	//	EMultiSampleCount sampleCount = EMultiSampleCount::SampleCount_1;
	//}mMultiSampleState;

	struct PipelineShaders
	{
		optional<shared_ptr<VulkanShader>> vertexShader;
		optional<shared_ptr<VulkanShader>> geometryShader;
		optional<shared_ptr<VulkanShader>> fragmentShader;
		//optional<shared_ptr<VulkanShader>> computeShader;
		optional<shared_ptr<VulkanShader>> tessellationControlerShader;
		optional<shared_ptr<VulkanShader>> tessellationEvaluationShader;
		TArray(VkPipelineShaderStageCreateInfo) GetShaderStages() const;
	}mShaders;
private:
	mutable VkPipelineRasterizationStateCreateInfo  mRasterizationState;
	mutable VkPipelineDepthStencilStateCreateInfo mDepthStencilState;
	mutable VkPipelineVertexInputStateCreateInfo mVertexInputState;
	mutable VkPipelineMultisampleStateCreateInfo mMultisampleState;
	mutable VkPipelineTessellationStateCreateInfo mTessellationState;
	mutable VkPipelineInputAssemblyStateCreateInfo  mInputAssemblyState;
	mutable TArray(VkPipelineColorBlendAttachmentState) mColorAttachmentsBlendStates;
	mutable VkPipelineColorBlendStateCreateInfo mColorBlendState;


	void InitVkPipelineStructures() const;
	void InitVertexInputState() const;
	void InitIAState() const;
	void InitRasterizationState() const;
	void InitDepthStencilState() const;
	void InitColorAttachementsBlendState() const;
};


class VulkanPipeline : public enable_shared_from_this<VulkanPipeline>
{
public:
	static shared_ptr<VulkanPipeline> GetOrCreateGraphicsPipelineState(const PipelineState& inPipelineState, shared_ptr<VulkanRenderPass> inRenderPass, shared_ptr<VulkanDevice> inDevice);
	~VulkanPipeline()
	{

	};
private:

	VulkanPipeline(VkPipeline inPipeline, PipelineState inPipelineState)
		: mVkPipeline(inPipeline)
		, mPipelineState(inPipelineState)
	{

	};

	DefineMemberWithGetter(VkPipeline, VkPipeline)
	PipelineState mPipelineState;
	//上一次创建PSO时的缓存数据，可以用来初始化下一次PSO创建
	//static std::vector<char*> mPipelineCachedData;
	//static std::map<PipelineState, shared_ptr<VulkanPipeline>> GCachedPipelineStates;
};
