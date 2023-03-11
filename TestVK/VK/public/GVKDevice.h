#pragma once
#include"GVKInstance.h"
#include "GVKQueue.h"
#include<vulkan/vulkan.hpp>
#include<iostream>
using namespace std;
class GVKQueue;
class GVKDevice : public shared_ptr<GVKDevice>
{
public:
	GVKDevice(GVKInstance Intsance);

	~GVKDevice();
	VkPhysicalDevice GetCurrentGPU() const;
private:
	//physicalDevieces
	std::vector<VkPhysicalDevice> mGPUs;
	int mCurrentGPUIndex = -1;
	//LogicDeviece
	VkDevice mDevice;
	GVKInstance mInstance;
	//显卡要求

	//GVKQueue
	shared_ptr<GVKQueue> mQueue;
private:
	void EnumerateGPUs();
	void SelectTargetGPU();
	bool IsGPUSuitable(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties* DeviceProperties, VkPhysicalDeviceFeatures* DeviceFeatures) const;
	void GetGPUPropertiseAndFeatures(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties& DeviceProperties, VkPhysicalDeviceFeatures& DeviceFeatures) const;
	//给显卡打分
	int	RateDeviceSuitability(VkPhysicalDevice* GPU) const;


	void CreateLogicalDevice()
	{
		VkDeviceCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		//CreateInfo.pQueueCreateInfos = ;
		//CreateInfo.queueCreateInfoCount = 1;
		//CreateInfo.pEnabledFeatures
	}
};
