#pragma once
#include "GVKQueue.h"
#include "GVKInstance.h"
#include<vulkan/vulkan.hpp>
#include<iostream>
using namespace std;
class GVKQueue;
class GVKInstance;
class GVKDevice : public shared_ptr<GVKDevice>
{
public:
	GVKDevice(GVKInstance* Intsance);

	~GVKDevice();
	
	void Cleanup();

	VkPhysicalDevice GetCurrentGPU() const;

	VkDevice GetVKDevice() const;

private:
	//physicalDevieces
	std::vector<VkPhysicalDevice> mGPUs;
	int mCurrentGPUIndex = -1;
	//LogicDeviece
	VkDevice mDevice;
	//weak_ptr<GVKInstance> mInstance;
	GVKInstance* mInstance;
	//�Կ�Ҫ��

	//GVKQueue
	GVKQueue* mQueue;
	//createInfo and createInfo Priorities
	std::vector<VkDeviceQueueCreateInfo> mQueueCreateInfos;
	std::vector<float> mQueuePrioritys;
	VkPhysicalDeviceFeatures mGPUFeatures = {};
private:
	void EnumerateGPUs();

	void SelectTargetGPU();

	bool IsGPUSuitable(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties* DeviceProperties, VkPhysicalDeviceFeatures* DeviceFeatures) const;
	
	void GetGPUPropertiseAndFeatures(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties& DeviceProperties, VkPhysicalDeviceFeatures& DeviceFeatures) const;
	//���Կ����
	int	RateDeviceSuitability(VkPhysicalDevice* GPU) const;

	void CreateLogicalDevice();
	
	void BuildVkDeviceQueueCreateInfo();

};
