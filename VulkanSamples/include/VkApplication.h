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
	* ����vulkanʵ��
	*/
	VkResult createVulkanInstance(std::vector<const char*>& layers,
								std::vector<const char*>& extensions,
								const char* appName);
	VkResult enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList);
	/*
	* ����Ҫѡ���gpuIndex��index����Ļ���ѡ��0
	*/
	VkResult initialize(int gpuIndex);
	VkResult handShakeWithDevice(VkPhysicalDevice *gpu,std::vector<const char*>&layers,std::vector<const char *> &extensions);
	void setInstanceExtensionNames(std::vector<const char*>& inInstanceExtensionNames);
	void setLayerNames(std::vector<const char*>& inLayerNames);
	//�����ж��Ƿ����Ҫ��
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