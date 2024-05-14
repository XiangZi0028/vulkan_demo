#include <iostream>
#include <VulkanRHI/public/VulkanRHI.h>
#include <VulkanRHI/public/VulkanCommandBuffer.h>
#include <VulkanRHI/public/VulkanPipeline.h>
#include <VulkanRHI/public/VulkanRenderPass.h>
#include <VulkanRHI/public/VulkanFrameBuffer.h>
#include <VulkanRHI/public/VulkanRenderTarget.h>
#include <VulkanRHI/public/VulkanShader.h>
#include <VulkanRHI/public/VulkanSwapChain.h>
#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanRHI/public/VulkanBarrierLayout.h>
#include <VulkanVertexBuffer.h>
#include <CommonMicro.h>
#include <vulkan/vulkan.h>
#include <ProjectInfoPath.h>
using namespace std;

int main()
{
	shared_ptr<VulkanCore> VkCore(new VulkanCore());
	VkCore->InitVKCore();
	shared_ptr<VulkanCommandPool> CommandBufferPool = VulkanCommandPool::CreateCommandPool(VkCore->GetDevice(), EVulkanCmdBufferPoolType::Graphics);
	shared_ptr<VulkanCommandBuffer> CommandBuffer = VulkanCommandBuffer::Create(CommandBufferPool);
	//创建顶点Buffer
	std::vector<float> Vertices = { 
				1.0f,  1.0f, 0.0f , 1.0f, 0.0f, 0.0f ,
			   -1.0f,  1.0f, 0.0f ,  0.0f, 1.0f, 0.0f ,
			    0.0f, -1.0f, 0.0f ,  0.0f, 0.0f, 1.0f 
	};
	std::vector<uint16_t> indices = { 0, 1, 2 };
	std::vector<EVertexIAType> VertexAttributeTypes;
	VertexAttributeTypes.push_back(IA_Position);
	VertexAttributeTypes.push_back(IA_Color);
	shared_ptr<VulkanVertexBuffer> VertexBuffer = VulkanVertexBuffer::Create(VkCore->GetDevice(), CommandBuffer, Vertices, VertexAttributeTypes);
	shared_ptr<VulkanIndexBuffer> IndexBuffer = VulkanIndexBuffer::Create(VkCore->GetDevice(), CommandBuffer, indices);
	
	//创建RenderTargets
	TArray(shared_ptr<VulkanRenderTarget>) RenderTargets;
	shared_ptr<VulkanRenderTarget> Depth = VulkanRenderTarget::CreateAttachment(EAttachmentType::Depth, VkCore->GetDevice(), EPixelFormat::PF_DepthStencil, 1024, 1024);
	shared_ptr<VulkanRenderTarget> Color1= VulkanRenderTarget::CreateAttachment(EAttachmentType::Color, VkCore->GetDevice(), EPixelFormat::PF_B8G8R8A8, 1024, 1024);
	shared_ptr<VulkanRenderTarget> Color2 = VulkanRenderTarget::CreateAttachment(EAttachmentType::Color, VkCore->GetDevice(), EPixelFormat::PF_B8G8R8A8, 1024, 1024);
	shared_ptr<VulkanRenderTarget> Color3 = VulkanRenderTarget::CreateAttachment(EAttachmentType::Color, VkCore->GetDevice(), EPixelFormat::PF_B8G8R8A8, 1024, 1024);
	RenderTargets.push_back(Color1);
	RenderTargets.push_back(Color2);
	RenderTargets.push_back(Color3);
	RenderTargets.push_back(Depth);

	shared_ptr<VulkanFrameBuffer> FrameBuffer = VulkanFrameBuffer::Create(VkCore->GetDevice(), RenderTargets);

	//创建RenderPass
	shared_ptr<VulkanRenderPass> RenderPass = VulkanRenderPass::Create(CommandBuffer, FrameBuffer, VkCore->GetSwapChain());

	//设置PipelineState
	shared_ptr<VulkanShader> Vertexshader = VulkanShader::CreateVertexShader(VkCore->GetDevice(), std::string(SHADER_PATH) + "/Triangle_vert.spv", "main");
	shared_ptr<VulkanShader> Fragmentshader = VulkanShader::CreateFragmentShader(VkCore->GetDevice(), std::string(SHADER_PATH) + "/Triangle_frag.spv", "main");

	//new GVKShader(ShaderType::VertexShader, );
	//mFragmentShader = new GVKShader(ShaderType::FragmentShader, std::string(SHADER_PATH) + "/Triangle_frag.spv");

	PipelineState CurPipelineState;
	CurPipelineState.mShaders.vertexShader = Vertexshader;
	CurPipelineState.mShaders.fragmentShader = Fragmentshader;
	CurPipelineState.mDepthAndStencilStateInitialzer.bEnableDepthTest = false;
	CurPipelineState.mDepthAndStencilStateInitialzer.bEnbaleBackStencilTest = false;
	CurPipelineState.mDepthAndStencilStateInitialzer.bEnbaleFrontStencilTest = false;

	//创建Pipeline
	shared_ptr<VulkanPipeline> Pipeline = VulkanPipeline::GetOrCreateGraphicsPipelineState(CurPipelineState, RenderPass , VkCore->GetDevice());
	
	Viewport viewPort(1024, 1024);
	CommandBuffer->BeginCommandBuffer();
	RenderPass->BeginRenderPass()
		->SetViewport(viewPort)
		->SetScissor(1024, 1024, 0, 0)
		->BindPipeline()
		->BindVertexBuffer(VertexBuffer)
		->BindIndexBuffer(IndexBuffer)
		->DrawVertex()
		->EndRenderPass();
	CommandBuffer->EndCommandBuffer();
	VkSemaphoreCreateInfo CreateInfo;
	ZeroVulkanStruct(CreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
	VkSemaphore CmdSemaphore;
	vkCreateSemaphore(VkCore->GetDevice()->GetDevice(), &CreateInfo, nullptr, &CmdSemaphore);
	CommandBuffer->SubmitCommandBuffer(1, &CmdSemaphore);
	//正常流程：创建RT的时候 有一张Image应该是作为Present使用的(这个图应该每帧去swap chain返回的images中去取)
	//这里先直接加一个Draw，将被标记为swapchain的图绘制到frame buffer上。
	while (true)
	{
		VkSemaphore Semaphore = VK_NULL_HANDLE;
		VkCore->GetSwapChain()->AcquireNextImage(Semaphore);
		int curBackBufferIndex = VkCore->GetSwapChain()->GetCurBackBufferIndex();
		VkImage curScreenImage = VkCore->GetSwapChain()->GetImages()[curBackBufferIndex];
		shared_ptr<VulkanImageView> curBackBufferImageView =  VkCore->GetSwapChain()->GetImageViews()[curBackBufferIndex];
		SubresourceRange subresourceRange = curBackBufferImageView->GetSubresourceRange();
		MemoryBarrierInfo screenImageMemoryBarrier;
		screenImageMemoryBarrier.mBarrierType = EPipelineBarrierType::PBT_ImageMemory;
		screenImageMemoryBarrier.mImage = curScreenImage;
		screenImageMemoryBarrier.mOldLayout = ETextureBarrierLayout::TBL_Undefined;
		screenImageMemoryBarrier.mNewLayout = ETextureBarrierLayout::TBL_TransferDst;
		screenImageMemoryBarrier.SrcPipelineStatge = EPipelineStageFlags::PSF_PipelineBottom;
		screenImageMemoryBarrier.DstPipelineStatge = EPipelineStageFlags::PSF_Transfer;
		screenImageMemoryBarrier.mBarrierCount = 1;
		screenImageMemoryBarrier.mSubResourceImage = subresourceRange;
		VulkanBarrierLayoutManager::BarrierLayoutExcute(CommandBuffer, screenImageMemoryBarrier);

		CommandBuffer->BeginCommandBuffer();
		VkImageCopy region;
		region.dstOffset = { 0,0,0 };
		region.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		region.extent = { 1024, 1024, 1 };
		region.srcOffset = { 0,0,0 };
		region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		vkCmdCopyImage(CommandBuffer->GetCommandBufferRef(), Color1->GetImage()->GetImg(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, curScreenImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		CommandBuffer->EndCommandBuffer();
		CommandBuffer->SubmitCommandBuffer();

		screenImageMemoryBarrier.mOldLayout= ETextureBarrierLayout::TBL_TransferDst;
		screenImageMemoryBarrier.mNewLayout = ETextureBarrierLayout::TBL_PresentSrc;
		screenImageMemoryBarrier.SrcPipelineStatge = EPipelineStageFlags::PSF_Transfer;
		screenImageMemoryBarrier.DstPipelineStatge = EPipelineStageFlags::PSF_PipelineBottom;
		VulkanBarrierLayoutManager::BarrierLayoutExcute(CommandBuffer, screenImageMemoryBarrier);
		VkCore->GetSwapChain()->Present(Semaphore);

	}
	return 0;
}