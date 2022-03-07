
#include<VkCommandBufferMgr.h>

CommdBufferMgr::CommdBufferMgr()
{
}

CommdBufferMgr::~CommdBufferMgr()
{
}
void CommdBufferMgr::allocCommandBuffer(const VkDevice* device, 
										const VkCommandPool cmdPoll, 
										const std::vector<VkCommandBuffer>& cmdBuf,
										VkCommandBufferAllocateInfo* inCmdBufInfo)
{
	VkResult res;
	// 如果用户指定了分配内存的方式信息 则采用指定的
	if (inCmdBufInfo)
	{
		res = vkAllocateCommandBuffers(device,inCmdBufInfo, cmdBuf.data());
		assert(res==VK_SUCCESS);
		return;
	}
	//否则采用默认指定分配方式来进行分配
	VkCommandBufferAllocateInfo cmdInfo = {};
	cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdInfo.commandPool = cmdPoll;
	cmdInfo.pNext = nullptr;
	cmdInfo.commandBufferCount = cmdBuf.size();
	cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	//分配空间
	res = vkAllocateCommandBuffers(device,&cmdInfo, cmdBuf.data());
	assert(res==VK_SUCCESS)
}
void CommdBufferMgr::beginCommandBuffer(VkCommandBuffer cmdBuf, 
										VkCommandBufferBeginInfo* inCmdBufInfo)
{
	VkResult res;
	//如果用户设置了自定义的指令缓存
	if (inCmdBufInfo)
	{
		res = vkBeginCommandBuffer(cmdBuf,inCmdBufInfo);
		assert(res == VK_SUCCESS)
			return;
	}
	//否则我们需要使用默认的方案
	VkCommandBufferInheritanceInfo cmdBufInheritInfo = {};
	cmdBufInheritInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	cmdBufInheritInfo.pNext = nullptr;
	cmdBufInheritInfo.occlusionQueryEnable = VK_FALSE;
	cmdBufInheritInfo.framebuffer = VK_NULL_HANDLE;
	cmdBufInheritInfo.renderPass = VK_NULL_HANDLE;
	cmdBufInheritInfo.queryFlags = 0;
	cmdBufInheritInfo.pipelineStatistics = 0;
	cmdBufInheritInfo.renderPass = 0;

	VkCommandBufferBeginInfo cmdBufInfo = {};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufInfo.pNext = nullptr;
	cmdBufInfo.pInheritanceInfo = cmdBufInheritInfo;
	cmdBufInfo.flags=0;

	res = vkBeginCommandBuffer(cmdBuf, &cmdBufInfo);
	assert(res==VK_SUCCESS)
}
void CommdBufferMgr::endCommandBuffer(VkCommandBuffer cmdBuf)
{
	VkResult res = vkEndCommandBuffer(cmdBuf);
	assert(res==VK_SUCCESS);
}
void CommdBufferMgr::submitCommandBuffer(const VkQueue& queue,
										 const std::vector<VkCommandBuffer>& cmdBufList,
										 const VkSubmitInfo* inSubmitInfo,
										 const VkFence& fence)
{
	VkResult res;
	//如果提交过程的控制属性可用的话，那么指令缓存已经被设置在其中了
	if (inSubmitInfo)
	{
		vkQueueSubmit(queue, 1, inSubmitInfo, fence);
		res = vkQueueWaitIdle(queue);
		return;
	}
	//创建默认的提交属性
	VkSubmitInfo submitInfo;
	submitInfo.commandBufferCount = cmdBufList.size();
	submitInfo.pCommandBuffers = cmdBufList.data();
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.pSignalSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.signalSemaphoreCount = 0;

	res = vkQueueSubmit(queue, 1, , &submitInfo, fence);
	assert(res == VK_SUCCESS);
	res = vkQueueWaitIdle(queue);
	assert(res == VK_SUCCESS);
}
1