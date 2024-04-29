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
#include "VulkanCommonDefine.h"
class VulkanDevice;
class VulkanSwapChain;
class VulkanCore : public enable_shared_from_this<VulkanCore>
{
public:
	VulkanCore()
	:mWindow(nullptr)
	{
	};
	
	~VulkanCore() {};
	void InitVKCore(uint32_t inWindowWith = 0, uint32_t inWindowHeight = 0, bool inEnableSurface = true);
	void ResetWindowExtent(uint32_t inWindowWith, uint32_t inWindowHeight) { mWindowWidth = inWindowWith;  mWindowHeight = inWindowHeight; };
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
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;
private:
	int mEnabledDeviceIndex = -1;
	shared_ptr<VulkanSwapChain> mSwapChain;
	VkPresentModeKHR mEnbalePresentMode;
	VkInstance mVkInstance;
	VkSurfaceKHR mVkSurface = nullptr;
	VkPhysicalDeviceFeatures mDeviceFeatures;
	TArray(const char*) mValidationLayers;
	TArray(const char*) mInstanceExtensions;
	TArray(VkExtensionProperties) mVKInstanceExtensions;
	TArray(VkLayerProperties) mVKLayerProperties;
	TArray(const char*) mDeviceExtensions;
	TArray(TArray(VkExtensionProperties)) mVkDeviceExtensions;
	TArray(std::shared_ptr<VulkanDevice>) mVulkanDevieces;
	DefineMemberWithGetterSetter(GLFWwindow*, Window)
	
};

