
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
	// ����û�ָ���˷����ڴ�ķ�ʽ��Ϣ �����ָ����
	if (inCmdBufInfo)
	{
		res = vkAllocateCommandBuffers(device,inCmdBufInfo, cmdBuf.data());
		assert(res==VK_SUCCESS);
		return;
	}
	//�������Ĭ��ָ�����䷽ʽ�����з���
	VkCommandBufferAllocateInfo cmdInfo = {};
	cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdInfo.commandPool = cmdPoll;
	cmdInfo.pNext = nullptr;
	cmdInfo.commandBufferCount = cmdBuf.size();
	cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	//����ռ�
	res = vkAllocateCommandBuffers(device,&cmdInfo, cmdBuf.data());
	assert(res==VK_SUCCESS)
}
void CommdBufferMgr::beginCommandBuffer(VkCommandBuffer cmdBuf, 
										VkCommandBufferBeginInfo* inCmdBufInfo)
{
	VkResult res;
	//����û��������Զ����ָ���
	if (inCmdBufInfo)
	{
		res = vkBeginCommandBuffer(cmdBuf,inCmdBufInfo);
		assert(res == VK_SUCCESS)
			return;
	}
	//����������Ҫʹ��Ĭ�ϵķ���
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
	//����ύ���̵Ŀ������Կ��õĻ�����ôָ����Ѿ���������������
	if (inSubmitInfo)
	{
		vkQueueSubmit(queue, 1, inSubmitInfo, fence);
		res = vkQueueWaitIdle(queue);
		return;
	}
	//����Ĭ�ϵ��ύ����
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