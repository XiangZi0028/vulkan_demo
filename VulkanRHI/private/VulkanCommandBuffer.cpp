#include <VulkanRHI/public/VulkanCommandBuffer.h>
#include <VulkanRHI/public/CommonMicro.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include "VulkanRHI/public/VulkanCommonDefine.h"
#include <vulkan/vulkan.h>

//VukanQueue
shared_ptr<VulkanQueue> VulkanQueue::CreateQueue(shared_ptr<VulkanDevice> inVulkanDevice, uint32_t inFamilyIndex)
{
	shared_ptr<VulkanQueue> newQueue(new VulkanQueue(inVulkanDevice, inFamilyIndex));
	vkGetDeviceQueue(inVulkanDevice->GetDevice(), inFamilyIndex, 0, &(newQueue->GetVkQueueRef()));
	return newQueue;
}
//VulkanCommandPool

shared_ptr<VulkanCommandPool> VulkanCommandPool::CreateCommandPool(shared_ptr<VulkanDevice> inDevice, EVulkanCmdBufferPoolType inDesiredPoolType)
{
	shared_ptr<VulkanCommandPool> newCommandPool(new VulkanCommandPool(inDevice));
	auto queueFamilyIndices = inDevice->GetQueueFamilyIndices();
	switch (inDesiredPoolType)
	{
		case EVulkanCmdBufferPoolType::Present:
		{
			if (queueFamilyIndices.PresentFamily.has_value())
			{
				newCommandPool->SetFamilyIndex(queueFamilyIndices.PresentFamily.value());
				break;
			}
		}
		case EVulkanCmdBufferPoolType::Transfer:
		{
			if (queueFamilyIndices.PresentFamily.has_value())
			{
				newCommandPool->SetFamilyIndex(queueFamilyIndices.TransferFamily.value());
				break;
			}
		}
		case EVulkanCmdBufferPoolType::Compute:
		{
			if (queueFamilyIndices.PresentFamily.has_value())
			{
				newCommandPool->SetFamilyIndex(queueFamilyIndices.ComputeFamily.value());
				break;
			}
		}
		default:
		newCommandPool->SetFamilyIndex(queueFamilyIndices.GraphicsFamily.value());
		break;
	}
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	ZeroVulkanStruct(commandPoolCreateInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
	commandPoolCreateInfo.queueFamilyIndex = inDevice->GetQueueFamilyIndices().GraphicsFamily.value();
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkCreateCommandPool(inDevice->GetDevice(), &commandPoolCreateInfo, nullptr, &(newCommandPool->GetVkCommandPoolRef()));

	return newCommandPool;
}

//VulkanCommandBuffer
//存在问题：这里虽然queue可以去拿默认的指定的队列，但是commandPool怎么办。commandpool按理说需要去拿和queue对应的才行
shared_ptr<VulkanCommandBuffer> VulkanCommandBuffer::Create(shared_ptr<VulkanCommandPool> inCommandPool, VkCommandBufferLevel inCommandBufferLevel, shared_ptr<VulkanQueue> inQueue)
{
	if (!inQueue)
	{
		inQueue = inCommandPool->mDevice->GetQueues().GraphicsQueue.value();
	}

	shared_ptr<VulkanCommandBuffer> newCommandBuffer(new VulkanCommandBuffer(inCommandPool->mDevice, inCommandPool, inCommandBufferLevel, inQueue));

	VkCommandBufferAllocateInfo commandBufferAllocateInfo;
	ZeroVulkanStruct(commandBufferAllocateInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO);
	commandBufferAllocateInfo.commandPool = inCommandPool->GetVkCommandPoolRef();
	commandBufferAllocateInfo.level = inCommandBufferLevel;
	commandBufferAllocateInfo.commandBufferCount = 1;
	vkAllocateCommandBuffers(inCommandPool->mDevice->GetDevice(), &commandBufferAllocateInfo, &(newCommandBuffer->GetCommandBufferRef()));
	
	VkFenceCreateInfo fenceCreateInfo;
	ZeroVulkanStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
	fenceCreateInfo.flags = 0;
	vkCreateFence(inCommandPool->mDevice->GetDevice(), &fenceCreateInfo, nullptr, &(newCommandBuffer->GetVkFenceRef()));
	inCommandPool->AddCommandBuffer(newCommandBuffer);
	return newCommandBuffer;
}

void VulkanCommandBuffer::BeginCommandBuffer()
{
	if (bIsBegun)
	{
		return;
	}
	VkCommandBufferBeginInfo beginCommandBufferInfo;
	ZeroVulkanStruct(beginCommandBufferInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO);
	beginCommandBufferInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;//VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;//VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(mCommandBuffer, &beginCommandBufferInfo);
	bIsBegun = true;
}

void VulkanCommandBuffer::EndCommandBuffer()
{
	if (!bIsBegun)
	{
		return;
	}
	vkEndCommandBuffer(mCommandBuffer);
	bIsBegun = false;
}

void VulkanCommandBuffer::SubmitCommandBuffer(uint32_t signalSemaphoreNum, VkSemaphore* signalSemaphore, uint32_t waitSemaphoreNum, VkSemaphore* waitSemaphore)
{
	//make sure before submit
	EndCommandBuffer();
	VkSubmitInfo submitInfo;
	ZeroVulkanStruct(submitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO);
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &mCommandBuffer;
	submitInfo.pSignalSemaphores = signalSemaphore;
	submitInfo.signalSemaphoreCount = signalSemaphoreNum;
	submitInfo.pWaitSemaphores = waitSemaphore;
	submitInfo.waitSemaphoreCount = waitSemaphoreNum;
	submitInfo.pWaitDstStageMask = nullptr;
	vkResetFences(mDevice->GetDevice(), 1, &mVkFence);
	vkQueueSubmit(mVulkanQueue->GetVkQueueRef() , 1, &submitInfo, mVkFence);
	vkWaitForFences(mDevice->GetDevice(), 1, &mVkFence, true, 1000);
}