#pragma once
#include "GVKQueue.h"
#include "GVKInstance.h"
#include<vulkan/vulkan.hpp>
#include<iostream>
using namespace std;
class GVKQueue;
class GVKInstance;
class GVKDevice : public enable_shared_from_this<GVKDevice>
{
public:
	GVKDevice(GVKInstance* Intsance);

	~GVKDevice();
	
	void Cleanup();

	VkPhysicalDevice GetCurrentGPU() const;

	VkDevice GetVKDevice() const;

	GVKInstance* mInstance;

private:
	//physicalDevieces
	std::vector<VkPhysicalDevice> mGPUs;
	int mCurrentGPUIndex = -1;
	//LogicDeviece
	VkDevice mDevice;
	//weak_ptr<GVKInstance> mInstance;
	//�Կ�Ҫ��
	//�豸��չ
	//GPUIndex Extensions
	std::vector<pair<int, std::vector<VkExtensionProperties>>> mAvailableDeviceExtensions_GPUs;
	std::vector<const char *> mRequiredDeviceExtensions;

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
	int	RateDeviceSuitability(int GPUIndex) const;

	void CreateLogicalDevice();
	
	void BuildVkDeviceQueueCreateInfo();

	void InitAvailableDeviceExtensions(int Index);

	void InitRequiredDeviceExtensions();

	bool CheckDeviceExtensionSupport(int GPUIndex) const;
};
