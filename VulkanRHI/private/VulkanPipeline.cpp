#include <VulkanRHI/public/VulkanPipeline.h>
#include <VulkanRHI/public/VulkanShader.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanRHI/public/VulkanRenderPass.h>
#include <VulkanRHI/public/VulkanFrameBuffer.h>
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>

std::vector<char*> PipelineCachedData;

TArray(VkPipelineShaderStageCreateInfo) PipelineState::PipelineShaders::GetShaderStages() const
{
	TArray(VkPipelineShaderStageCreateInfo) allStages;
	if (vertexShader.has_value())
	{
		allStages.push_back(vertexShader.value()->GetShaderStageInfo());
	}
	if (fragmentShader.has_value())
	{
		allStages.push_back(fragmentShader.value()->GetShaderStageInfo());
	}
	return allStages;
}

void PipelineState::InitIAState() const
{
	ZeroVulkanStruct(mInputAssemblyState, VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
	mInputAssemblyState.topology = (VkPrimitiveTopology)mPrimitiveType;
}

void PipelineState::InitRasterizationState() const
{
	auto Initializer = mRasterizerStateInitialzer;

	ZeroVulkanStruct(mRasterizationState, VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
	//mRasterizationState.depthClampEnable = VK_FALSE;
	//mRasterizationState.rasterizerDiscardEnable = VK_FALSE;
	auto RasterizerFillModeToVulkan = [](ERasterizerFillMode inFillMode)
	{
		switch (inFillMode)
		{
			case Point:			return VK_POLYGON_MODE_POINT;
			case Line:		return VK_POLYGON_MODE_LINE;
			case Fill:			return VK_POLYGON_MODE_FILL;
			default:
				break;
		}
		return VK_POLYGON_MODE_MAX_ENUM;
	};

	auto RasterizerCullModeToVulkan = [](ECullMode inCullMode)
	{
		switch (inCullMode)
		{
		case CM_None:			return VK_CULL_MODE_NONE;
		case CM_CW:				return VK_CULL_MODE_FRONT_BIT;
		case CM_CCW:			return VK_CULL_MODE_BACK_BIT;
		default:
			break;
		}
		return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
	};

	mRasterizationState.polygonMode = RasterizerFillModeToVulkan(Initializer.FillMode);
	mRasterizationState.cullMode = RasterizerCullModeToVulkan(Initializer.CullMode);
	mRasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
	mRasterizationState.depthBiasEnable = Initializer.DepthBias == 0 ? VK_FALSE : VK_TRUE;
	mRasterizationState.depthBiasConstantFactor = Initializer.DepthBias;
	mRasterizationState.depthBiasSlopeFactor = Initializer.SlopScaleDepthBias;
	mRasterizationState.lineWidth = 1.0;
	//mRasterizationState.depthBiasClamp;
}

void PipelineState::InitDepthStencilState() const
{
	ZeroVulkanStruct(mDepthStencilState, VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
	auto Initializer = mDepthAndStencilStateInitialzer;
	mDepthStencilState.depthTestEnable = (Initializer.depthCompareOp != ECompareOp::Always || Initializer.bEnableDepthTest) ? VK_TRUE : VK_FALSE;
	mDepthStencilState.depthWriteEnable = Initializer.bEnableWriteDepth = Initializer.bEnableWriteDepth ? VK_TRUE : VK_FALSE;;
	auto CompareOpToVulkan = [](ECompareOp InOp)
	{
		switch (InOp)
		{
			case Less:			return VK_COMPARE_OP_LESS;
			case LessOrEqual:		return VK_COMPARE_OP_LESS_OR_EQUAL;
			case Greater:		return VK_COMPARE_OP_GREATER;
			case GreaterOrEqual:	return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case Equal:			return VK_COMPARE_OP_EQUAL;
			case NotEqual:		return VK_COMPARE_OP_NOT_EQUAL;
			case Never:			return VK_COMPARE_OP_NEVER;
			case Always:			return VK_COMPARE_OP_ALWAYS;
			default:
			break;
		}
		return VK_COMPARE_OP_MAX_ENUM;
	};
	mDepthStencilState.depthCompareOp = CompareOpToVulkan(Initializer.depthCompareOp);
	{
		mDepthStencilState.depthBoundsTestEnable = bDepthBounds;
		mDepthStencilState.minDepthBounds = 0.0f;
		mDepthStencilState.maxDepthBounds = 1.0f;
	}
	mDepthStencilState.stencilTestEnable = (Initializer.bEnbaleFrontStencilTest || Initializer.bEnbaleBackStencilTest) ? VK_TRUE : VK_FALSE;;
	auto StencilOpToVulkan = [](EStencilOp inStencilOp)
	{
		switch (inStencilOp)
		{
			case Keep:					return VK_STENCIL_OP_KEEP;
			case Zero:					return VK_STENCIL_OP_ZERO;
			case Replace:				return VK_STENCIL_OP_REPLACE;
			case IncrementAndClamp:		return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case DecrementAndClamp:		return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case Invert:					return VK_STENCIL_OP_INVERT;
			case IncrementAndWarp:				return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case DecrementAndWarp:				return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			default:
				break;
		}
		return VK_STENCIL_OP_MAX_ENUM;
	};

	mDepthStencilState.front.failOp = StencilOpToVulkan(Initializer.stencilFailOp_Front);
	mDepthStencilState.front.passOp = StencilOpToVulkan(Initializer.stencilPassOp_Front);
	mDepthStencilState.front.depthFailOp = StencilOpToVulkan(Initializer.depthFailStencilOp_Front);
	mDepthStencilState.front.compareOp = CompareOpToVulkan(Initializer.stencilCompareOp_Front);
	mDepthStencilState.front.compareMask = Initializer.compareMask;
	mDepthStencilState.front.writeMask = Initializer.writeMask;
	mDepthStencilState.front.reference = 0;

	if (Initializer.bEnbaleBackStencilTest)
	{
		mDepthStencilState.back.failOp = StencilOpToVulkan(Initializer.stencilFailOp_Back);
		mDepthStencilState.back.passOp = StencilOpToVulkan(Initializer.stencilPassOp_Back);
		mDepthStencilState.back.depthFailOp = StencilOpToVulkan(Initializer.depthFailStencilOp_Back);
		mDepthStencilState.back.compareOp = CompareOpToVulkan(Initializer.stencilCompareOp_Back);
		mDepthStencilState.back.compareMask = Initializer.compareMask;
		mDepthStencilState.back.writeMask = Initializer.writeMask;
		mDepthStencilState.back.reference = 0;
	}
	else
	{
		mDepthStencilState.back = mDepthStencilState.front;
	}
}

void PipelineState::InitVertexInputState() const
{
	//Test

	VkVertexInputBindingDescription vertexInputBinding = {};
	vertexInputBinding.binding = 0; // Vertex Buffer 0
	vertexInputBinding.stride = sizeof(float) * 6; // Position + Color
	vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributs(2);
	// position
	vertexInputAttributs[0].binding = 0;
	vertexInputAttributs[0].location = 0; // triangle.vert : layout (location = 0)
	vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributs[0].offset = 0;
	// color
	vertexInputAttributs[1].binding = 0;
	vertexInputAttributs[1].location = 1; // triangle.vert : layout (location = 1)
	vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributs[1].offset = 12;
	/*if (mShaders.vertexShader.has_value())
	{
		mVertexInputState.
	}*/
	ZeroVulkanStruct(mVertexInputState, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
	mVertexInputState.vertexBindingDescriptionCount = 1;
	mVertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
	mVertexInputState.vertexAttributeDescriptionCount = 2;
	mVertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();
}

void PipelineState::InitColorAttachementsBlendState() const
{
	auto BlendOpToVulkan = [](EBlendOp inOp)
	{
		switch (inOp)
		{
			case BO_Add:				return VK_BLEND_OP_ADD;
			case BO_Subtract:			return VK_BLEND_OP_SUBTRACT;
			case BO_Min:				return VK_BLEND_OP_MIN;
			case BO_Max:				return VK_BLEND_OP_MAX;
			case BO_ReverseSubtract:	return VK_BLEND_OP_REVERSE_SUBTRACT;
			default:
			break;
		}
		return VK_BLEND_OP_MAX_ENUM;
	};

	auto BlendFactorToVulkan = [](EBlendFactor inBlendFactor)
	{
		switch (inBlendFactor)
		{
		case BF_Zero:						return VK_BLEND_FACTOR_ZERO;
		case BF_One:						return VK_BLEND_FACTOR_ONE;
		case BF_SourceColor:				return VK_BLEND_FACTOR_SRC_COLOR;
		case BF_InverseSourceColor:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case BF_SourceAlpha:				return VK_BLEND_FACTOR_SRC_ALPHA;
		case BF_InverseSourceAlpha:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case BF_DestAlpha:					return VK_BLEND_FACTOR_DST_ALPHA;
		case BF_InverseDestAlpha:			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case BF_DestColor:					return VK_BLEND_FACTOR_DST_COLOR;
		case BF_InverseDestColor:			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case BF_ConstantBlendFactor:		return VK_BLEND_FACTOR_CONSTANT_COLOR;
		case BF_InverseConstantBlendFactor:	return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		case BF_Source1Color:				return VK_BLEND_FACTOR_SRC1_COLOR;
		case BF_InverseSource1Color:		return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
		case BF_Source1Alpha:				return VK_BLEND_FACTOR_SRC1_ALPHA;
		case BF_InverseSource1Alpha:		return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
		default:
			break;
		}
		return VK_BLEND_FACTOR_MAX_ENUM;
	};
	for (size_t Index = 0; Index < MaxSimultaneousRenderTargets; Index++)
	{
		auto Initializer = mBlendStateInitialzers[Index];
		VkPipelineColorBlendAttachmentState ColorBlend;
		//ZeroVulkanStruct(ColorBlend, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
		ColorBlend.blendEnable = Initializer.bEnableBlend ?  VK_TRUE : VK_FALSE;

		ColorBlend.alphaBlendOp = BlendOpToVulkan(Initializer.alphaOp);
		ColorBlend.srcAlphaBlendFactor = BlendFactorToVulkan(Initializer.alphaSrcFactor);
		ColorBlend.dstAlphaBlendFactor = BlendFactorToVulkan(Initializer.alphaDstFactor);

		ColorBlend.colorBlendOp = BlendOpToVulkan(Initializer.colorOp);
		ColorBlend.srcColorBlendFactor = BlendFactorToVulkan(Initializer.colorSrcFactor);
		ColorBlend.dstColorBlendFactor = BlendFactorToVulkan(Initializer.colorDstFactor);
		ColorBlend.colorWriteMask = (Initializer.writeMask & CW_RED) ? VK_COLOR_COMPONENT_R_BIT : 0;
		ColorBlend.colorWriteMask |= (Initializer.writeMask & CW_GREEN) ? VK_COLOR_COMPONENT_G_BIT : 0;
		ColorBlend.colorWriteMask |= (Initializer.writeMask & CW_BLUE) ? VK_COLOR_COMPONENT_B_BIT : 0;
		ColorBlend.colorWriteMask |= (Initializer.writeMask & CW_ALPHA) ? VK_COLOR_COMPONENT_A_BIT : 0;
		mColorAttachmentsBlendStates.push_back(ColorBlend);
	}
	ZeroVulkanStruct(mColorBlendState, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
	mColorBlendState.attachmentCount = 3;
	mColorBlendState.pAttachments = mColorAttachmentsBlendStates.data();
}

void PipelineState::InitVkPipelineStructures() const
{
	InitVertexInputState();
	InitIAState();
	InitRasterizationState();
	InitDepthStencilState();
	InitColorAttachementsBlendState();
}

shared_ptr<VulkanPipeline> VulkanPipeline::GetOrCreateGraphicsPipelineState(const PipelineState& inPipelineState, shared_ptr<VulkanRenderPass> inRenderPass,shared_ptr<VulkanDevice> inDevice)
{
	//存储方式有问题，需要一个健全的利用结构体生成key的方式
	/*auto it = GCachedPipelineStates.find(inPipelineState);
	if (it != GCachedPipelineStates.end())
	{
		return nullptr;
	}*/
	VkPipelineCache pipelineCache;
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo;
	ZeroVulkanStruct(pipelineCacheCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO);
	pipelineCacheCreateInfo.pInitialData = PipelineCachedData.data();
	pipelineCacheCreateInfo.initialDataSize = PipelineCachedData.size();
	vkCreatePipelineCache(inDevice->GetDevice(), &pipelineCacheCreateInfo, nullptr, &pipelineCache);
	inPipelineState.InitVkPipelineStructures();

	VkGraphicsPipelineCreateInfo graphicPipelineCreateInfo;
	ZeroVulkanStruct(graphicPipelineCreateInfo, VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);
	graphicPipelineCreateInfo.layout;
	graphicPipelineCreateInfo.renderPass = inRenderPass->GetVkRenderPass();
	auto shaderStages = inPipelineState.mShaders.GetShaderStages();
	graphicPipelineCreateInfo.stageCount = shaderStages.size();
	graphicPipelineCreateInfo.pStages = shaderStages.data();
	graphicPipelineCreateInfo.pInputAssemblyState = &inPipelineState.mInputAssemblyState;


	VkVertexInputBindingDescription vertexInputBinding = {};
	vertexInputBinding.binding = 0; // Vertex Buffer 0
	vertexInputBinding.stride = sizeof(float) * 6; // Position + Color
	vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributs(2);
	// position
	vertexInputAttributs[0].binding = 0;
	vertexInputAttributs[0].location = 0; // triangle.vert : layout (location = 0)
	vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributs[0].offset = 0;
	// color
	vertexInputAttributs[1].binding = 0;
	vertexInputAttributs[1].location = 1; // triangle.vert : layout (location = 1)
	vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributs[1].offset = 12;
	/*if (mShaders.vertexShader.has_value())
	{
		mVertexInputState.
	}*/
	ZeroVulkanStruct(inPipelineState.mVertexInputState, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
	inPipelineState.mVertexInputState.vertexBindingDescriptionCount = 1;
	inPipelineState.mVertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
	inPipelineState.mVertexInputState.vertexAttributeDescriptionCount = 2;
	inPipelineState.mVertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();
	graphicPipelineCreateInfo.pVertexInputState= &inPipelineState.mVertexInputState;


	graphicPipelineCreateInfo.pRasterizationState = &inPipelineState.mRasterizationState;
	graphicPipelineCreateInfo.pDepthStencilState = &inPipelineState.mDepthStencilState;
	graphicPipelineCreateInfo.pColorBlendState = &inPipelineState.mColorBlendState;
	
	//viewport和scissor state不重要，因为我们将它设置为动态的，在cmd中去动态设置就行
	{
		VkPipelineViewportStateCreateInfo viewportState;
		ZeroVulkanStruct(viewportState, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkViewport viewport = {};
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = 0;
		viewport.height = 0;
		viewport.minDepth = 0;
		viewport.maxDepth = 1;
		viewportState.pViewports = &viewport;


		VkRect2D scissor = {};
		scissor.extent.width = 0; inRenderPass->GetFrameBuffer()->GetFrameBufferWidth();
		scissor.extent.height = 0; inRenderPass->GetFrameBuffer()->GetFrameBufferHeight();
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		viewportState.pScissors = &scissor;

		graphicPipelineCreateInfo.pViewportState = &viewportState;
	}

	
	std::vector<VkDynamicState> dynamicStateEnables;
	dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
	VkPipelineDynamicStateCreateInfo dynamicState;
	ZeroVulkanStruct(dynamicState, VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
	dynamicState.dynamicStateCount = (uint32_t)dynamicStateEnables.size();
	dynamicState.pDynamicStates = dynamicStateEnables.data();
	graphicPipelineCreateInfo.pDynamicState = &dynamicState;


	VkDescriptorSetLayoutBinding layoutBinding;
	layoutBinding.binding = 0;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers = nullptr;
	VkDescriptorSetLayout           m_DescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet                 m_DescriptorSet = VK_NULL_HANDLE;
	VkPipelineLayout                m_PipelineLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayoutCreateInfo descSetLayoutInfo;
	ZeroVulkanStruct(descSetLayoutInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO);
	descSetLayoutInfo.bindingCount = 1;
	descSetLayoutInfo.pBindings = &layoutBinding;
	vkCreateDescriptorSetLayout(inDevice->GetDevice(), &descSetLayoutInfo, nullptr, &m_DescriptorSetLayout);
	VkPipelineLayoutCreateInfo pipeLayoutInfo;
	ZeroVulkanStruct(pipeLayoutInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
	pipeLayoutInfo.setLayoutCount = 1;
	pipeLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
	vkCreatePipelineLayout(inDevice->GetDevice(), &pipeLayoutInfo, nullptr, &m_PipelineLayout);
	graphicPipelineCreateInfo.layout = m_PipelineLayout;

	VkPipeline vkPipeline = VK_NULL_HANDLE;
	vkCreateGraphicsPipelines(inDevice->GetDevice(), VK_NULL_HANDLE, 1, &graphicPipelineCreateInfo, nullptr, &vkPipeline);
	shared_ptr<VulkanPipeline> newPipeline(new VulkanPipeline(vkPipeline, inPipelineState));
	inRenderPass->mPipeline = newPipeline;
	return newPipeline;
}