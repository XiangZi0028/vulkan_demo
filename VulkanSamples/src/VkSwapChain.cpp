#include<VkSwapChain.h>
#define INSTANCE_FUNC_PTR(instance,entrypoint){\
fp##entrypoint=(PFN_vk_##entrypoint) vkGetInstanceProcAddr(intsance,"vk"#entrypoint);\
if(fp##entrypoint==nullptr){\
std::cout<<"Unable to locate the vkGetInstanceProcAddr: vk"#entrypoint<<std::endl;\
	}\
}
#define DEVICE_FUBC_PTR(dev,entrypoint){\
fp##entrrpoint=(PFN_vk##entrypoint) vkGetDeviceProcAddr(instance,"vk"#entrypoint);\
if(fp##entrypoint==nullptr){\
std::cout<<"Uable to locate the vkGetDeviceProcAddr: vk"#entrypoint<<std::endl;\
}\
}
VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::~VulkanSwapChain()
{
}