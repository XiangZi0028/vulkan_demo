#pragma once
class CommdBufferMgr
{
public:
	CommdBufferMgr();
	~CommdBufferMgr();
	//从指令池分配内存到指令缓存
	static void allocCommandBuffer(const VkDevice* device, 
								   const VkCommandPool cmdPoll, 
								  const std::vector<VkCommandBuffer>& cmdBuf,
								   VkCommandBufferAllocateInfo* inCmdBufInfo = nullptr);
	//开始进行指令缓存的录制
	static void beginCommandBuffer(VkCommandBuffer cmdBuf, 
								   VkCommandBufferBeginInfo* inCmdBufInfo = nullptr);
	//结束指令缓存的录制
	static void endCommandBuffer(VkCommandBuffer cmdBuf);
	//将指令缓存提交到执行部分
	static void submitCommandBuffer(const VkQueue &queue,
									const std::vector<VkCommandBuffer>& cmdBufList,
									const VkSubmitInfo* inSubmitInfo = nullptr,
									const VkFence& fence = VK_NULL_HANDLE);
private:

};
