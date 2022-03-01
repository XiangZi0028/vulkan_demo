#pragma once
#include<VkInstance.h>
#include<VkLayerAndExtension.h>
#include<iostream>
#include<memory>
#include<mutex>
#include<VKDevice.h>


class VulkanApplication
{
private:
	VulkanApplication();
public:
	~VulkanApplication();
	static VulkanApplication* GetInstance();
	/*
	* 创建vulkan实列
	*/
	VkResult createVulkanInstance(std::vector<const char*>& layers,
								std::vector<const char*>& extensions,
								const char* appName);
	VkResult enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList);
	VkResult initialize();
	VkResult handShakeWithDevice(VkPhysicalDevice *gpu,std::vector<const char*>&layers,std::vector<const char *> &extensions);

private:
	static std::once_flag _onlyOnce;
	static std::unique_ptr<VulkanApplication> _Instance;
	bool debugFlag;
public:
	VulkanInstance _instanceObj;

	VulkanDevice* _deviceObj;
};