#pragma once
class CommdBufferMgr
{
public:
	CommdBufferMgr();
	~CommdBufferMgr();
	//��ָ��ط����ڴ浽ָ���
	static void allocCommandBuffer(const VkDevice* device, 
								   const VkCommandPool cmdPoll, 
								  const std::vector<VkCommandBuffer>& cmdBuf,
								   VkCommandBufferAllocateInfo* inCmdBufInfo = nullptr);
	//��ʼ����ָ����¼��
	static void beginCommandBuffer(VkCommandBuffer cmdBuf, 
								   VkCommandBufferBeginInfo* inCmdBufInfo = nullptr);
	//����ָ����¼��
	static void endCommandBuffer(VkCommandBuffer cmdBuf);
	//��ָ����ύ��ִ�в���
	static void submitCommandBuffer(const VkQueue &queue,
									const std::vector<VkCommandBuffer>& cmdBufList,
									const VkSubmitInfo* inSubmitInfo = nullptr,
									const VkFence& fence = VK_NULL_HANDLE);
private:

};
