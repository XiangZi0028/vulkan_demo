#include <VulkanRHI/public/VulkanBarrierLayout.h>
#include <VulkanRHI/public/VulkanCommandBuffer.h>
#include <vulkan/vulkan.h>
#include <queue>
#include <VulkanRHI/public/CommonMicro.h>
#include <VulkanRHI/public/VulkanCommonDefine.h>
void VulkanBarrierLayoutManager::BarrierLayoutExcute(shared_ptr<VulkanCommandBuffer> inCmdBuffer, MemoryBarrierInfo& inMemoryBarrier)
{
	inCmdBuffer->BeginCommandBuffer();
	switch (inMemoryBarrier.mBarrierType)
	{
	case EPipelineBarrierType::PBT_BufferMemory:
	{
		VkBufferMemoryBarrier memoryBarrier = GetBufferMemoryBarrier(inMemoryBarrier);
		vkCmdPipelineBarrier(inCmdBuffer->GetCommandBufferRef(), ConvertPipelineStageFlagesToVk(inMemoryBarrier.SrcPipelineStatge), ConvertPipelineStageFlagesToVk(inMemoryBarrier.DstPipelineStatge), 0, 0, nullptr, inMemoryBarrier.mBarrierCount, &memoryBarrier, 0, nullptr);
	};
	break;
	case EPipelineBarrierType::PBT_ImageMemory:
	{
		VkImageMemoryBarrier memoryBarrier = GetImageMemoryBarrier(inMemoryBarrier);
		vkCmdPipelineBarrier(inCmdBuffer->GetCommandBufferRef(), ConvertPipelineStageFlagesToVk(inMemoryBarrier.SrcPipelineStatge), ConvertPipelineStageFlagesToVk(inMemoryBarrier.DstPipelineStatge), 0, 0, nullptr, 0, nullptr, inMemoryBarrier.mBarrierCount, &memoryBarrier);
	};
	break;
	case EPipelineBarrierType::PBT_Memory: break;
	{
		VkMemoryBarrier memoryBarrier = GetMemoryBarrier(inMemoryBarrier);
		vkCmdPipelineBarrier(inCmdBuffer->GetCommandBufferRef(), ConvertPipelineStageFlagesToVk(inMemoryBarrier.SrcPipelineStatge), ConvertPipelineStageFlagesToVk(inMemoryBarrier.DstPipelineStatge), 0, inMemoryBarrier.mBarrierCount, &memoryBarrier, 0, nullptr, 0, nullptr);
	};
	default:
	{
		return;
	}
	}
	inCmdBuffer->EndCommandBuffer();
	inCmdBuffer->SubmitCommandBuffer();
};

bool VulkanBarrierLayoutManager::DequeueMemoryBarriers()
{
	if (mMemoryBarriers.size())
	{
		MemoryBarrierInfo memoryBarriers = mMemoryBarriers.front();
		BarrierLayoutExcute(mCmdBuffer, memoryBarriers);
		mMemoryBarriers.pop();
		return true;
	}
		return false;
}

bool VulkanBarrierLayoutManager::DequeueImageMemoryBarriers()
{
	if (mImageMemoryBarriers.size())
	{
		MemoryBarrierInfo memoryBarriers = mImageMemoryBarriers.front();
		BarrierLayoutExcute(mCmdBuffer, memoryBarriers);
		mMemoryBarriers.pop();
	}
	return false;
}

bool VulkanBarrierLayoutManager::DequeueBufferMemoryBarriers()
{
	if (mBufferMemoryBarriers.size())
	{
		MemoryBarrierInfo memoryBarriers = mBufferMemoryBarriers.front();
		BarrierLayoutExcute(mCmdBuffer, memoryBarriers);
		mMemoryBarriers.pop();
	}
	return false;
}


VkImageMemoryBarrier VulkanBarrierLayoutManager::GetImageMemoryBarrier(MemoryBarrierInfo& inMemoryBarrier)
{
	VkImageMemoryBarrier imageMemroyBarrier;
	ZeroVulkanStruct(imageMemroyBarrier, VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER);
	imageMemroyBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemroyBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemroyBarrier.subresourceRange.aspectMask = (VkImageAspectFlagBits)inMemoryBarrier.mSubResourceImage.mImageAspect;
	imageMemroyBarrier.subresourceRange.baseArrayLayer = inMemoryBarrier.mSubResourceImage.mBaseArrayLayer;
	imageMemroyBarrier.subresourceRange.layerCount = inMemoryBarrier.mSubResourceImage.mLayerCount;
	imageMemroyBarrier.subresourceRange.levelCount = inMemoryBarrier.mSubResourceImage.mLevelCount;
	imageMemroyBarrier.subresourceRange.baseMipLevel = inMemoryBarrier.mSubResourceImage.mBaseMipLevel;
	imageMemroyBarrier.image = inMemoryBarrier.mImage;
	imageMemroyBarrier.newLayout = ConvertTextureBarrierLayoutToVk(inMemoryBarrier.mNewLayout);
	imageMemroyBarrier.oldLayout = ConvertTextureBarrierLayoutToVk(inMemoryBarrier.mOldLayout);
	VkPipelineStageFlags sourceStages = ConvertPipelineStageFlagesToVk(inMemoryBarrier.SrcPipelineStatge);
	VkPipelineStageFlags dstStages = ConvertPipelineStageFlagesToVk(inMemoryBarrier.DstPipelineStatge);
	return imageMemroyBarrier;
}

VkBufferMemoryBarrier VulkanBarrierLayoutManager::GetBufferMemoryBarrier(MemoryBarrierInfo& inMemoryBarrier)
{
	VkBufferMemoryBarrier bufferMemroyBarrier;
	return bufferMemroyBarrier;
}

VkMemoryBarrier VulkanBarrierLayoutManager::GetMemoryBarrier(MemoryBarrierInfo& inMemoryBarrier)
{
	//Todo
	VkMemoryBarrier memoryBarrier;
	return memoryBarrier;
}

VulkanBarrierLayoutManager::VulkanBarrierLayoutManager()
{

}

VulkanBarrierLayoutManager::~VulkanBarrierLayoutManager()
{

}

VkPipelineStageFlags VulkanBarrierLayoutManager::ConvertPipelineStageFlagesToVk(EPipelineStageFlags inPipelineStageFlags)
{
	switch (inPipelineStageFlags)
	{
		case PSF_PipelineTop:
			return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		case PSF_PipelineBottom:
			return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		case PSF_Transfer:
			return VK_PIPELINE_STAGE_TRANSFER_BIT;
		case PSF_ColorAttachmentOutput:
			return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		default:
			return 0;
			break;
	}
	return 0;
}

VkImageLayout VulkanBarrierLayoutManager::ConvertTextureBarrierLayoutToVk(ETextureBarrierLayout inTextureBarrierLayout)
{
	switch (inTextureBarrierLayout)
	{
		case TBL_TransferSrc:
		{
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		}
		case TBL_TransferDst:
		{
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		}
		case TBL_ColorAttachment:
		{
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		//todo :important
		case TBL_DepthStencilAttachment:
		{
			return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
		}	
		case TBL_PresentSrc:
		{
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		}
		case TBL_Undefined:
		default:
		{
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
			break;
	}
}