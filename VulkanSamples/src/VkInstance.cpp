#include<VkInstance.h>

VulkanInstance::VulkanInstance()
{
	_instance = {};
}

VulkanInstance::~VulkanInstance()
{
}

VkResult VulkanInstance::createInstance(std::vector<const char*>& layers, 
										std::vector<const char*>& extensions,
										const char* appName)
{
	//设置实例相关的层和扩展的信息
	_layerExtension._appRequestedextensionNames = extensions;
	_layerExtension._appRequestedlayerNames = layers;

	//定义Vulkan应用程序的结构体
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = appName;
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_API_VERSION_1_2;

	//定义vulkan实例创建的参数结构体
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = &_layerExtension._debugReportCreateInfo;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;

	//设置需要启用的层名称的列表
	instanceInfo.enabledLayerCount = (uint32_t)layers.size();
	instanceInfo.ppEnabledLayerNames = layers.size() ? layers.data() : nullptr;

	//设置需要启用的扩展列表
	instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.size() ? extensions.data() : nullptr;

	// pAllocator 设置了宿主机分配内存的方法
	VkResult res = vkCreateInstance(&instanceInfo,nullptr,&_instance);
	return res;
}

void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(_instance,nullptr);
}
