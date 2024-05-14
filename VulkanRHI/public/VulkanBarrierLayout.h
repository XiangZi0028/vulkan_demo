#pragma once
#include <VulkanCommonDefine.h>
#include <CommonMicro.h>
#include <vulkan/vulkan.h>

enum EPipelineBarrierType
{
	PBT_Memory = 0,
	PBT_ImageMemory = 1,
	PBT_BufferMemory = 2,
	PBT_MaxNum = 3
};
enum EPipelineStageFlags
{
	PSF_PipelineTop = 0,
	PSF_PipelineBottom = 1,
	PSF_Transfer = 2,
	PSF_ColorAttachmentOutput = 3
};

struct MemoryBarrierInfo
{
	MemoryBarrierInfo()
	{

	};
	EPipelineStageFlags SrcPipelineStatge;
	EPipelineStageFlags DstPipelineStatge;
	EPipelineBarrierType mBarrierType;
	uint32_t mBarrierCount;
	ETextureBarrierLayout mOldLayout;
	ETextureBarrierLayout mNewLayout;
	SubresourceRange mSubResourceImage;
	VkImage mImage = VK_NULL_HANDLE;
	VkBuffer mBuffer = VK_NULL_HANDLE;
	//void* mMemoryBarriers;
};
class VulkanCommandBuffer;
class VulkanBarrierLayoutManager : public enable_shared_from_this<VulkanBarrierLayoutManager>
{
public:
	VulkanBarrierLayoutManager();
	~VulkanBarrierLayoutManager();
	
	void EnqueueMemoryBarriers(MemoryBarrierInfo& inMemoryBarrier) {};

	bool DequeueMemoryBarriers();
	bool DequeueImageMemoryBarriers();
	bool DequeueBufferMemoryBarriers();
	//Static IMPL
	static VkImageMemoryBarrier GetImageMemoryBarrier(MemoryBarrierInfo& inMemoryBarrier);
	static VkBufferMemoryBarrier GetBufferMemoryBarrier(MemoryBarrierInfo& inMemoryBarrier);
	static VkMemoryBarrier GetMemoryBarrier(MemoryBarrierInfo& inMemoryBarrier);
	static VkPipelineStageFlags ConvertPipelineStageFlagesToVk(EPipelineStageFlags inPipelineStageFlags);
	static VkImageLayout ConvertTextureBarrierLayoutToVk(ETextureBarrierLayout inTextureBarrierLayout);

	static void BarrierLayoutExcute(shared_ptr<VulkanCommandBuffer> inCmdBuffer, MemoryBarrierInfo& inMemoryBarrier);
private:

	TQueue(MemoryBarrierInfo) mMemoryBarriers;
	TQueue(MemoryBarrierInfo) mImageMemoryBarriers;
	TQueue(MemoryBarrierInfo) mBufferMemoryBarriers;

	shared_ptr<VulkanCommandBuffer> mCmdBuffer = nullptr;
};
