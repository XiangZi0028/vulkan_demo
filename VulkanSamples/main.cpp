#include<VkApplication.h>
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


int main(int argc,char **argv)
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	appObj->setInstanceExtensionNames(instanceExtensionNames);
	appObj->setLayerNames(layerNames);
	appObj->initialize(0);
	VulkanRender* Renderer= new VulkanRender(appObj,appObj->_deviceObj);
	Renderer->createPresentationWindow(500, 500);

	return 0;
}