#pragma once
#ifndef VLUKANDEMO_GVKINSTANCE_H
#define VLUKANDEMO_GVKINSTANCE_H
#endif
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include "CommonMicro.h"
class VulkanDevice;
class VulkanCore : public enable_shared_from_this<VulkanCore>
{
public:
	VulkanCore() {};
	
	~VulkanCore() {};
	void InitVKCore();
private:
	// Begine Create VkInstance
	void CreateInstance();
	void InitInstanceValidationLayers();
	void InitRequiredInstanceExtensions();
	void EnumerateInstanceExtensionProperties();
	void EnumerateInstanceLayerProperties();
	bool CheckValidationLayersSupport();
	bool CheckExtensionsSupports();
	// End Create VkInstance

	void CreateSurface();

	//Begin Selcted GPU And Create Deviece
	void SelecteGPUAndCreateDevice();
	void InitDeivceExtensions() {};
	void InitDeviceFeatures() {};
	void CreateSwapChain();
protected:
	bool bEnableVsync = false;

private:
	std::shared_ptr<>
	VkPresentModeKHR mEnbalePresentMode;
	VkInstance mVkInstance;
	VkSurfaceKHR mVkSurface;
	TArray(const char*) mValidationLayers;
	TArray(const char*) mInstanceExtensions;
	TArray(VkExtensionProperties) mVKInstanceExtensions;
	TArray(VkLayerProperties) mVKLayerProperties;
	TArray(const char*) mDeviceExtensions;
	TArray(TArray(VkExtensionProperties)) mVkDeviceExtensions;
	TArray(std::shared_ptr<VulkanDevice>) mVulkanDevieces;
	VkPhysicalDeviceFeatures mDeviceFeatures;
	int mEnabledDeviceIndex = -1;
	DefineMemberWithGetterSetter(GLFWwindow*, Window)
};

