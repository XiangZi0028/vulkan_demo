#pragma once
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>
class VulkanCommandBuffer;
class VulkanSwapChain;
class VulkanFrameBuffer;
class VulkanPipeline;
class VulkanVertexBuffer;
class VulkanIndexBuffer;
struct Viewport {
	Viewport(int inWidth, int inHeight) {
		width = inWidth;
		height = inHeight;
	};
	int    x = 0;
	int    y = 0;
	int    width;
	int    height;
	float    minDepth = 0;
	float    maxDepth = 1;
};

class VulkanRenderPass : public enable_shared_from_this<VulkanRenderPass>
{
public:
	
	~VulkanRenderPass() 
	{

	};
	static shared_ptr<VulkanRenderPass> Create(shared_ptr <VulkanCommandBuffer> inCommandBuffer, shared_ptr <VulkanFrameBuffer> inFrameBuffer, shared_ptr <VulkanSwapChain> inSwapChain);
	shared_ptr<VulkanRenderPass> BeginRenderPass();
	shared_ptr<VulkanRenderPass> SetViewport(const Viewport& inViewPort);
	shared_ptr<VulkanRenderPass> SetScissor(int width, int height, int offsetX = 0, int offsetY = 0);
	shared_ptr<VulkanRenderPass> BindPipeline();
	shared_ptr<VulkanRenderPass> BindVertexBuffer(shared_ptr<VulkanVertexBuffer> inVertexBuffer);
	shared_ptr<VulkanRenderPass> BindIndexBuffer(shared_ptr<VulkanIndexBuffer> inVertexBuffer);
	shared_ptr<VulkanRenderPass> DrawIndex(uint32_t instanceCount = 1);
	shared_ptr<VulkanRenderPass> DrawVertex(uint32_t instanceCount = 1);
	shared_ptr<VulkanRenderPass> AddSubPass(TArray(int) inputAttachemntIndex, TArray(int) OutputAttachemntIndex);
	shared_ptr<VulkanRenderPass> EndRenderPass();
	void Init();
	friend class VulkanPipeline;
private:
	VulkanRenderPass(shared_ptr <VulkanCommandBuffer> inCommandBuffer, shared_ptr <VulkanFrameBuffer> inFrameBuffer, shared_ptr <VulkanSwapChain> inSwapChain)
		: mCommandBuffer(inCommandBuffer)
		, mFrameBuffer(inFrameBuffer)
		, mSwapChain(inSwapChain)
	{

	};

	typedef TArray(int) AttachemntsIndexes;
	shared_ptr <VulkanSwapChain> mSwapChain = nullptr;
	shared_ptr <VulkanCommandBuffer> mCommandBuffer = nullptr;
	DefineMemberDefaultWithGetter(shared_ptr <VulkanFrameBuffer>, FrameBuffer, nullptr)
	shared_ptr <VulkanVertexBuffer> mVertexBuffer = nullptr;
	shared_ptr <VulkanIndexBuffer> mIndexBuffer = nullptr;
	DefineMemberWithGetter(VkRenderPass, VkRenderPass)
	DefineMemberDefaultWithGetter(shared_ptr <VulkanPipeline>, Pipeline, nullptr)
	TArray(VkRenderPassCreateInfo) SubPassInfo;
	TArray(VkClearValue) mClearValues;
	/*TArray(AttachemntsIndexes) mInputAttachments;
	TArray(AttachemntsIndexes) mOutputAttachments;*/
};
