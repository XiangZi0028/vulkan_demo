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
	bool bEnableExtensions = true;
	bool bEnableValidationLayers = true;
public:
	GVKInstance(GLFWwindow *Window);
	~GVKInstance();
	VkInstance GetVKInstance();
	bool CheckValidationLayersSupport();
	void Cleanup();
	const std::vector<VkExtensionProperties>& GetExtensions()const { return mExtensions_VK; }
	const std::vector<const char*>& GetValidationLayers()const { return mValidationLayers; }
private:
	GVKSurfaceKHR* mGVKSurfaceKHR;
	GVKDevice* mGVKDevice;
	VkInstance mInstance;
	GLFWwindow* mWindow;
	string mAppNmae;
	string mEngineName;
	uint32_t mExtensionCount_glfw = 0;
	const char** mExtensions_glfw;
	std::vector<VkExtensionProperties> mExtensions_VK;
	std::vector<VkLayerProperties> mAvailableLayers_VK;
	std::vector<const char*> mValidationLayers;
private:
	void EnumerateInstanceExtensionProperties();
	void EnumerateInstanceLayerProperties();
	void CreateVKInstance();
	void InitValidationLayers();
};

