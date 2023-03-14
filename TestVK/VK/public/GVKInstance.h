#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Application.h"
#include<vulkan/vulkan.hpp>
#include<iostream>
#include "GVKSurfaceKHR.h"
#include "GVKDevice.h"
using namespace std;

class GVKDevice;
class GVKInstance : public shared_ptr<GVKInstance>
{
public:
	GVKSurfaceKHR* mGVKSurfaceKHR;
	bool bEnableExtensions = true;
	bool bEnableValidationLayers = true;
public:
	GVKInstance(GLFWwindow *Window);
	~GVKInstance();
	VkInstance GetVKInstance();
	bool CheckValidationLayersSupport();
	bool CheckExtensionsSupports();
	void Cleanup();
	const std::vector<VkExtensionProperties>& GetExtensions()const { return mInstanceExtensions_VK; }
	const std::vector<const char*>& GetValidationLayers()const { return mValidationLayers; }
private:
	GVKDevice* mGVKDevice;
	VkInstance mInstance;
	GLFWwindow* mWindow;
	string mAppNmae;
	string mEngineName;
	uint32_t mExtensionCount_glfw = 0;
	const char** mExtensions_glfw;
	//vk可能支持的extension属性
	std::vector<VkExtensionProperties> mInstanceExtensions_VK;
	//vk可能支持的layer属性
	std::vector<VkLayerProperties> mAvailableLayers_VK;
	//需要用到的extension属性
	std::vector<const char*> mInstanceExtensions;
	//需要用到的Lyaer属性
	std::vector<const char*> mValidationLayers;
private:
	void EnumerateInstanceExtensionProperties();
	void EnumerateInstanceLayerProperties();
	void CreateVKInstance();
	void InitInstanceValidationLayers();
	void InitRequiredInstanceExtensions();
};

