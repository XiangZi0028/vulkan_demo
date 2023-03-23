#pragma once
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include<iostream>
#include<map>
#include<vector>
#include <memory>

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
using GPUSwapchainSupportDetailsMap = map<VkPhysicalDevice, SwapChainSupportDetails*>;
class GVKSurfaceKHR;
class GVKSurfaceKHR : public enable_shared_from_this<GVKSurfaceKHR>
{
public:
	GVKSurfaceKHR();
	~GVKSurfaceKHR();
	void CreatePlatformSurfaceKHR(const VkInstance Instance, GLFWwindow* Window);
	SwapChainSupportDetails* GetPhysicalDeviceSurfaceSupportInfos(VkPhysicalDevice Device);
	void Cleanup(const VkInstance Instance);
	VkSurfaceKHR GetSUrface() { return mSurface; };

public:

private:
	GPUSwapchainSupportDetailsMap GPUSwapchainSupportDetails;
	VkSurfaceKHR mSurface;
	VkSurfaceCapabilitiesKHR mCapabilities;
	std::vector<VkSurfaceFormatKHR> mFormats;
	std::vector<VkPresentModeKHR> mPresentModes;	
};

