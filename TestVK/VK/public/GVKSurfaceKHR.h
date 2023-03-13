#pragma once
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include<iostream>
using namespace std;


class GVKSurfaceKHR : shared_ptr<GVKSurfaceKHR>
{
public:
	GVKSurfaceKHR();
	~GVKSurfaceKHR();
	void CreatePlatformSurfaceKHR(const VkInstance Instance, GLFWwindow* Window);
	void Cleanup(const VkInstance Instance);
private:

	VkSurfaceKHR mSurface;
};

