#include<VkSwapChain.h>
#include<vulkan/vulkan_win32.h>
#define INSTANCE_FUNC_PTR(instance, entrypoint){											\
    fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(instance, "vk"#entrypoint); \
    if (fp##entrypoint == NULL) {															\
        std::cout << "Unable to locate the vkGetDeviceProcAddr: vk"#entrypoint;				\
        exit(-1);																			\
    }																						\
}

#define DEVICE_FUNC_PTR(dev, entrypoint){													\
    fp##entrypoint = (PFN_vk##entrypoint) vkGetDeviceProcAddr(dev, "vk"#entrypoint);		\
    if (fp##entrypoint == NULL) {															\
        std::cout << "Unable to locate the vkGetDeviceProcAddr: vk"#entrypoint;				\
        exit(-1);																			\
    }																						\
}
VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::~VulkanSwapChain()
{
}

VkResult VulkanSwapChain::createSwapChainExtensions()
{
	VkResult res;
	 //获取基于设备和基于实例的函数指针会使用到实例对象和设备
	VkInstance& instance=_appObj-> _instanceObj._instance;
	VkDevice& device =_appObj->_deviceObj->_device;

	INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfaceSupportKHR);
	INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
	INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
	INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfacePresentModesKHR);
	INSTANCE_FUNC_PTR(instance, DestroySurfaceKHR);

	DEVICE_FUNC_PTR(device, CreateSwapchainKHR);
	DEVICE_FUNC_PTR(device, DestroySwapchainKHR);
	DEVICE_FUNC_PTR(device, GetSwapchainImagesKHR);
	DEVICE_FUNC_PTR(device, AcquireNextImageKHR);
	DEVICE_FUNC_PTR(device, QueuePresentKHR);

}

VkResult VulkanSwapChain::createSurface()
{
	VkResult res;
	VkInstance& instance = _appObj->_instanceObj._instance;

	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hinstance = _rendererObj->_connection;
	createInfo.hwnd = _rendererObj->_window;
	res = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &_surface);
	return res;
}
