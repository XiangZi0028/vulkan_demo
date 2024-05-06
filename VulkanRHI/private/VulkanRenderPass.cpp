#include <VulkanRHI/public/VulkanRenderPass.h>
#include <VulkanRHI/public/VulkanCommandBuffer.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanRHI/public/VulkanSwapChain.h>
#include <VulkanRHI/public/VulkanFrameBuffer.h>
#include <VulkanRHI/public/VulkanRenderTarget.h>
#include <VulkanRHI/public/VulkanPipeline.h>
#include <VulkanRHI/public/VulkanVertexBuffer.h>
#include <VulkanPipeline.h>
#include <vulkan/vulkan.h>

shared_ptr<VulkanRenderPass> VulkanRenderPass::Create(shared_ptr <VulkanCommandBuffer> inCommandBuffer, shared_ptr <VulkanFrameBuffer> inFrameBuffer, shared_ptr <VulkanSwapChain> inSwapChain)
{
	shared_ptr<VulkanRenderPass> newRenderPass(new VulkanRenderPass(inCommandBuffer, inFrameBuffer, inSwapChain));
	newRenderPass->Init();
	return newRenderPass;
}
void VulkanRenderPass::Init()
{
	std::vector<VkAttachmentReference> colorReferences;
	VkAttachmentReference depthReference;
	TArray(VkAttachmentDescription) attachmentDescs;// = mFrameBuffer->GetAttachmentDescs();
	int Index = 0;
	for (auto rt : mFrameBuffer->GetRenderTargets())
	{
		if (rt == nullptr) continue;
		bool bDepthStencilRT = false;
		VkAttachmentDescription attachementDesc = rt->GetAttachmentDescription(bDepthStencilRT);
		attachmentDescs.push_back(attachementDesc);
		VkAttachmentReference curReference = { };
		curReference.attachment = Index;
		if (bDepthStencilRT)
		{
			curReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			depthReference = curReference;
			mClearValues.push_back({ 1.0, 0 });
		}
		else
		{
			curReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorReferences.push_back(curReference);
			mClearValues.push_back({ 0.2f, 0.2f, 0.2f, 0.0f });
		}
		Index++;
	}
	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = colorReferences.size();
	subpassDescription.pColorAttachments = colorReferences.data();
	subpassDescription.pDepthStencilAttachment = &depthReference;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


	VkRenderPassCreateInfo renderPassInfo;
	ZeroVulkanStruct(renderPassInfo, VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO);
	renderPassInfo.attachmentCount = attachmentDescs.size();
	renderPassInfo.pAttachments = attachmentDescs.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescription;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &subpassDependency;
	vkCreateRenderPass(mCommandBuffer->GetDevice()->GetDevice(), &renderPassInfo, nullptr, &mVkRenderPass);
	mFrameBuffer->InitFrameBuffer(mVkRenderPass);
}
shared_ptr<VulkanRenderPass> VulkanRenderPass::BeginRenderPass()
{
	VkRenderPassBeginInfo renderPassBeginInfo;
	ZeroVulkanStruct(renderPassBeginInfo, VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO);
	renderPassBeginInfo.clearValueCount = mClearValues.size();
	renderPassBeginInfo.pClearValues = mClearValues.data();
	renderPassBeginInfo.renderPass = mVkRenderPass;
	renderPassBeginInfo.framebuffer = mFrameBuffer->GetFrameBuffer();
	renderPassBeginInfo.renderArea = VkRect2D{ VkOffset2D{0, 0},VkExtent2D {mFrameBuffer->GetFrameBufferWidth(), mFrameBuffer->GetFrameBufferHeight()}};
	vkCmdBeginRenderPass(mCommandBuffer->GetCommandBufferRef(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	return shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::SetViewport(const Viewport& inViewPort)
{
	VkViewport viewport = {};
	viewport.x = inViewPort.x;
	viewport.y = inViewPort.y;
	viewport.width = inViewPort.width;
	viewport.height = inViewPort.height;    // flip y axis
	viewport.minDepth = inViewPort.minDepth;
	viewport.maxDepth = inViewPort.maxDepth;
	vkCmdSetViewport(mCommandBuffer->GetCommandBufferRef(), 0, 1, &viewport);
	return shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::SetScissor(int offsetX, int offsetY, int width, int height)
{
	VkRect2D scissor = {};
	scissor.extent.width = 1024;
	scissor.extent.height = 1024;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	vkCmdSetScissor(mCommandBuffer->GetCommandBufferRef(), 0, 1, &scissor);
	return shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::BindPipeline()
{
	vkCmdBindPipeline(mCommandBuffer->GetCommandBufferRef(), VK_PIPELINE_BIND_POINT_GRAPHICS , mPipeline->GetVkPipeline());
	return this->shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::BindVertexBuffer(shared_ptr<VulkanVertexBuffer> inVertexBuffer)
{
	mVertexBuffer = inVertexBuffer;
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(mCommandBuffer->GetCommandBufferRef(), 0, 1, &inVertexBuffer->GetVulkanResourceBuffer()->GetVkBufferRef(), &offset);
	return shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::BindIndexBuffer(shared_ptr<VulkanIndexBuffer> inIndexBuffer)
{
	mIndexBuffer = inIndexBuffer;
	vkCmdBindIndexBuffer(mCommandBuffer->GetCommandBufferRef(), inIndexBuffer->GetVulkanResourceBuffer()->GetVkBufferRef(), 0, inIndexBuffer->GetVkIndexType());
	return shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::DrawIndex(uint32_t instanceCount)
{
	uint32_t IndicesCount = 0;
	IndicesCount = mIndexBuffer->GetIndexCount();
	vkCmdDrawIndexed(mCommandBuffer->GetCommandBufferRef(), IndicesCount, 1, 0, 0, 0);
	return this->shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::DrawVertex(uint32_t instanceCount)
{
	vkCmdDraw(mCommandBuffer->GetCommandBufferRef(), mVertexBuffer->GetVertexCount(), 1, 0, 0);
	return this->shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::AddSubPass(TArray(int) inputAttachemntIndex, TArray(int) OutputAttachemntIndex)
{
	/*mInputAttachments.push_back(inputAttachemntIndex);
	mOutputAttachments.push_back(inputAttachemntIndex);*/

	//std::vector<VkAttachmentReference> colorReferences;
	//VkAttachmentReference depthReference;

	//for (auto Index : inputAttachemntIndex)
	//{
	//	shared_ptr<VulkanRenderTarget> attachemnt = mFrameBuffer->GetRT(Index);
	//	if (attachemnt)
	//	{
	//		bool bDepthStencilAttachment = false;
	//		VkAttachmentDescription attachemntDesc = attachemnt->GetAttachmentDescription(bDepthStencilAttachment);
	//		VkAttachmentReference swapReference = { };
	//		if (!bDepthStencilAttachment)
	//		{
	//			swapReference.attachment = Index;
	//			swapReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//			colorReferences.push_back(swapReference);
	//		}
	//		else
	//		{
	//			depthReference.attachment = Index;
	//			depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	//		}
	//	}
	//}
	//VkSubpassDescription subpassDescription = {};
	//subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	//subpassDescription.colorAttachmentCount = colorReferences.size();
	//subpassDescription.pColorAttachments = colorReferences.data();
	//subpassDescription.pDepthStencilAttachment = &depthReference;

	return this->shared_from_this();
}

shared_ptr<VulkanRenderPass> VulkanRenderPass::EndRenderPass()
{
	vkCmdEndRenderPass(mCommandBuffer->GetCommandBufferRef());
	return this->shared_from_this();
}