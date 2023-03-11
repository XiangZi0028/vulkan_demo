#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include<vulkan/vulkan.hpp>
#include<iostream>
using namespace std;
class GVKInstance : public shared_ptr<GVKInstance>
{
public:
	GVKInstance();
	~GVKInstance();
	VkInstance GetVKInstance();
	bool CheckValidationLayersSupport();
	void DestroyGVKInstance();

private:
	VkInstance mInstance;
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

