#pragma once
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include<iostream>
#include<map>
#include<vector>
using namespace std;
struct SwapChainSupportDetails
{
	SwapChainSupportDetails()
	{
		mCapabilities = VkSurfaceCapabilitiesKHR{};
		/*mFormats.resize(0);
		mPresentModes.resize(0);*/
	}
	VkSurfaceCapabilitiesKHR mCapabilities;
	std::vector<VkSurfaceFormatKHR> mFormats;
	std::vector<VkPresentModeKHR> mPresentModes;
};
class GVKSurfaceKHR : shared_ptr<GVKSurfaceKHR>
{
public:
	GVKSurfaceKHR();
	~GVKSurfaceKHR();
	void CreatePlatformSurfaceKHR(const VkInstance Instance, GLFWwindow* Window);
	void GetPhysicalDeviceSurfaceSupportInfos(VkPhysicalDevice Device);
	void Cleanup(const VkInstance Instance);
	VkSurfaceKHR GetSUrface() { return mSurface; };

public:

private:
	map<VkDevice, SwapChainSupportDetails> GPUSwapchainSupportDetails;
	VkSurfaceKHR mSurface;
	VkSurfaceCapabilitiesKHR mCapabilities;
	std::vector<VkSurfaceFormatKHR> mFormats;
	std::vector<VkPresentModeKHR> mPresentModes;	
};

