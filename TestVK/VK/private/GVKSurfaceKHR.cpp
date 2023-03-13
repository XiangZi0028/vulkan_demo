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