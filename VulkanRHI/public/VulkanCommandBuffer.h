#pragma once
#include "CommonMicro.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommonDefine.h"
#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanQueue : public enable_shared_from_this<VulkanQueue>
{
public:
	~VulkanQueue() {};

	static shared_ptr<VulkanQueue> CreateQueue(shared_ptr<VulkanDevice> inVulkanDevice, uint32_t familyIndex);
	
private:
	VulkanQueue(shared_ptr<VulkanDevice> inVulkanDevice, uint32_t inFamilyIndex)
	: mDevice(inVulkanDevice),
	  mFamilyIndex(inFamilyIndex)
	
	{};

	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithGetter(uint32_t, FamilyIndex);
	DefineMemberWithRefGetter(VkQueue, VkQueue);
};

class VulkanCommandPool;
class VulkanCommandBuffer : public enable_shared_from_this<VulkanCommandBuffer>
{
public:
	~VulkanCommandBuffer() {};

	static shared_ptr<VulkanCommandBuffer> Create(shared_ptr<VulkanCommandPool> inCommandPool, VkCommandBufferLevel inCommandBufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY, shared_ptr<VulkanQueue> inQueue = nullptr);
	void BeginCommandBuffer();
	void EndCommandBuffer();
	void SubmitCommandBuffer(VkSemaphore* signalSemaphore = nullptr);
private:
	VulkanCommandBuffer(shared_ptr<VulkanDevice> inDevice, shared_ptr<VulkanCommandPool> inCommandPool, VkCommandBufferLevel inCommandBufferLevel, shared_ptr<VulkanQueue> inQueue)
		: mDevice(inDevice),
		mCommandPool(inCommandPool),
		mCommandBufferLevel(inCommandBufferLevel),
		mVulkanQueue(inQueue)
	{};
	DefineMemberWithGetter(shared_ptr<VulkanCommandPool>, CommandPool);
	DefineMemberWithGetter(VkCommandBufferLevel, CommandBufferLevel);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithRefGetter(VkFence, VkFence);
	DefineMemberWithRefGetter(VkCommandBuffer, CommandBuffer);
	DefineMemberWithGetter(shared_ptr<VulkanQueue>, VulkanQueue);

	//State Check
	bool bIsBegun = false;
};


class VulkanCommandPool : public enable_shared_from_this<VulkanCommandPool>
{
public:
	~VulkanCommandPool()
	{

	};
	static shared_ptr<VulkanCommandPool> CreateCommandPool(shared_ptr<VulkanDevice> inDevice, EVulkanCmdBufferPoolType inDesiredPoolType = Graphics);
	//friend
private:
	friend class VulkanCommandBuffer;
	VulkanCommandPool(shared_ptr<VulkanDevice> inDevice)
		:mDevice(inDevice)
	{

	};
	void AddCommandBuffer(shared_ptr<VulkanCommandBuffer> inCommandBuffer){
		mCommandBuffers.push_back(inCommandBuffer);
	};
	DefineMemberWithRefGetter(VkCommandPool, VkCommandPool);
	TArray(shared_ptr<VulkanCommandBuffer>) mCommandBuffers;
	DefineMemberWithGetterSetter(uint32_t, FamilyIndex);
	shared_ptr<VulkanDevice> mDevice;
};

