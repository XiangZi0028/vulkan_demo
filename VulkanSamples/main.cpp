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
	appObj->initialize();
	return 0;
}
VkResult VulkanApplication::initialize()
{
	char title[] = "Hello World!!!";

	if (debugFlag)
	{
		_instanceObj._layerExtension.areLayersSupported(layerNames);
	}

	//����vulkanʵ�� ������ָ���Ĳ����չ������
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	if (debugFlag) {
		_instanceObj._layerExtension.createDebugReportCallback();
	}

	//��ȡϵͳ�������豸���б�
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);

	//������ֻ�õ��˵�һ�����õ��豸
	if (gpuList.size() >= 0)
	{
		handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
	}
	return VK_SUCCESS;
}