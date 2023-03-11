#pragma
#include"GVKInstance.h"
#include<vulkan/vulkan.hpp>
#include<iostream>
using namespace std;

class GVKDevice : public shared_ptr<GVKDevice>
{
public:
	GVKDevice(GVKInstance Intsance);
	~GVKDevice();
	VkPhysicalDevice GetCurrentDevice() const;
private:
	//physicalDevieces
	std::vector<VkPhysicalDevice> mGPUs;
	int mCurrentGPUIndex = -1;
	//LogicDeviece
	VkDevice mDevice;
	GVKInstance mInstance;
	//显卡要求
private:
	void EnumerateGPUs();
	void SelectTargetGPU();
	bool IsGPUSuitable(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties* DeviceProperties, VkPhysicalDeviceFeatures* DeviceFeatures) const;
	void GetGPUPropertiseAndFeatures(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties& DeviceProperties, VkPhysicalDeviceFeatures& DeviceFeatures) const;
	//给显卡打分
	int	RateDeviceSuitability(VkPhysicalDevice* GPU) const;
};
