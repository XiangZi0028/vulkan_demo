#pragma once
#ifndef VLUKANDEMO_GVKINSTANCE_H
#define VLUKANDEMO_GVKINSTANCE_H

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Application.h"
#include<vulkan/vulkan.hpp>
#include<iostream>
#include "GVKSurfaceKHR.h"
#include "GVKDevice.h"
#include <memory>
using namespace std;

class GVKDevice;
class GVKSwapChain;
class GVKInstance : public enable_shared_from_this<GVKInstance>
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
    GVKSwapChain *mSwapChain;
	VkInstance mInstance;
	GLFWwindow* mWindow;
	string mAppNmae;
	string mEngineName;
	uint32_t mExtensionCount_glfw = 0;
	const char** mExtensions_glfw;
	//vk����֧�ֵ�extension����
	std::vector<VkExtensionProperties> mInstanceExtensions_VK;
	//vk����֧�ֵ�layer����
	std::vector<VkLayerProperties> mAvailableLayers_VK;
	//��Ҫ�õ���extension����
	std::vector<const char*> mInstanceExtensions;
	//��Ҫ�õ���Lyaer����
	std::vector<const char*> mValidationLayers;
private:
	void EnumerateInstanceExtensionProperties();
	void EnumerateInstanceLayerProperties();
	void CreateVKInstance();
	void InitInstanceValidationLayers();
	void InitRequiredInstanceExtensions();
};

#endif //VLUKANDEMO_GVKINSTANCE_H
