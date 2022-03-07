#include<vulkan/vulkan.h>

std::vector<const char *> instanceExtensionNames = { VK_KHR_SURFACE_EXTENSION_NAME,
													"VK_KHR_win32_surface",
													 VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
std::vector<const char*> layerNames = { 
										"VK_LAYER_LUNARG_api_dump",
										//"VK_LAYER_KHRONOS_validation",								
};
std::vector<const char*> deviceExtensionNames = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void createInstance()
{
	VkApplicationInfo appInfo = {};
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	createInfo.pApplicationInfo=
	vkCreateInstance( , );
}

int main(int argc,char **argv)
{
}