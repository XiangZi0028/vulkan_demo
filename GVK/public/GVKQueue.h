#pragma once
#include <vulkan/vulkan.hpp>
#include "GVKDevice.h"
#include<optional>
#include <iostream>
using namespace std;
class GVKDevice;
struct QueueFamilyIndices {
	std::optional<uint32_t> TransferFamily;
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> ComputeFamily;
	std::optional<uint32_t> PresentFamily;
};
class GVKQueue
{
public:
	GVKQueue(GVKDevice* Device);
	~GVKQueue();
	void InitGVKQueue();
    QueueFamilyIndices GetQueueFamilyIndices();
	void CreateDeviceQueue();
	VkQueue GetGraphicQueue() { return mGraphicQueue; };
	VkQueue GetTransferQueue() { return mTransferQueue; };
	VkQueue GetCompuyeQueue() { return mComputeQueue; }
    VkQueue GetPresentQueue() { return mComputeQueue; }

private:
	VkQueue mGraphicQueue;
	VkQueue mTransferQueue;
	VkQueue mComputeQueue;
	VkQueue mPresentQueue;
	std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
	GVKDevice* mDevice;
	QueueFamilyIndices mQueueFamilyIndics;
private:
	void GetGPUQueueFamilyProperties();
	void InitQueueFamilyIndices();
};

