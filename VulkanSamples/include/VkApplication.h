#pragma once
#include<VkInstance.h>
#include<VkLayerAndExtension.h>
#include<VkRenderer.h>
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
	/*
	* 传入要选择的gpuIndex，index错误的化就选择0
	*/
	VkResult initialize(int gpuIndex);
	VkResult handShakeWithDevice(VkPhysicalDevice *gpu,std::vector<const char*>&layers,std::vector<const char *> &extensions);
	void setInstanceExtensionNames(std::vector<const char*>& inInstanceExtensionNames);
	void setLayerNames(std::vector<const char*>& inLayerNames);
	//用来判断是否符合要求
	bool isDeviceSuitable(VkPhysicalDevice gpu);
private:
	static std::once_flag _onlyOnce;
	static std::unique_ptr<VulkanApplication> _Instance;
	std::vector<const char*> _instanceExtensionNames;
	std::vector<const char*> _layerNames;
	bool debugFlag;
public:
	VulkanInstance _instanceObj;

	VulkanDevice* _deviceObj;

	VulkanRender* _rendererObj;
};