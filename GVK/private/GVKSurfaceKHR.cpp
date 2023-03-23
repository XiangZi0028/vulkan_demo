#include "GVKSurfaceKHR.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
GVKSurfaceKHR::GVKSurfaceKHR()
{
}

GVKSurfaceKHR::~GVKSurfaceKHR()
{
}

void GVKSurfaceKHR::CreatePlatformSurfaceKHR(const VkInstance Instance,GLFWwindow* Window)
{
#if _WIN32
	VkWin32SurfaceCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	CreateInfo.hinstance = GetModuleHandle(nullptr);
	CreateInfo.hwnd = glfwGetWin32Window(Window);
	if (vkCreateWin32SurfaceKHR(Instance, &CreateInfo, nullptr, &mSurface))
	{
		throw std::runtime_error("Faild to create window surface!");
	}
#endif // _WIN32

}

void GVKSurfaceKHR::Cleanup(const VkInstance Instance)
{
	vkDestroySurfaceKHR(Instance, mSurface, nullptr);
}

SwapChainSupportDetails* GVKSurfaceKHR::GetPhysicalDeviceSurfaceSupportInfos(VkPhysicalDevice Device)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, mSurface, &mCapabilities);

	SwapChainSupportDetails* Details = nullptr;
	GPUSwapchainSupportDetailsMap::iterator iter = GPUSwapchainSupportDetails.find(Device);
	if (iter == GPUSwapchainSupportDetails.end())
	{
		GPUSwapchainSupportDetails[Device] = new SwapChainSupportDetails();
		Details = GPUSwapchainSupportDetails[Device];
		//GPUSwapchainSupportDetails.insert(GPUSwapchainSupportDetailsMap::value_type(Device, new SwapChainSupportDetails()));
	}
	else
	{
		Details = iter->second;
	}
	//PhysicalDevice Surface Format KHR
	{
		uint32_t FormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device, mSurface, &FormatCount, nullptr);
		if (FormatCount != 0)
		{
			
			Details->mFormats.resize(FormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(Device, mSurface, &FormatCount, Details->mFormats.data());
		}
	}

	//PhysicalDevice Surface PresentMode KHR
	{
		uint32_t PresentCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device, mSurface, &PresentCount, nullptr);
		if (PresentCount != 0) 
		{
			Details->mPresentModes.resize(PresentCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(Device, mSurface, &PresentCount, Details->mPresentModes.data());
		}
	}
	return Details;
}