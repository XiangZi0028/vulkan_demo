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
	//����ʵ����صĲ����չ����Ϣ
	_layerExtension._appRequestedextensionNames = extensions;
	_layerExtension._appRequestedlayerNames = layers;

	//����VulkanӦ�ó���Ľṹ��
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = appName;
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_API_VERSION_1_2;

	//����vulkanʵ�������Ĳ����ṹ��
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = &_layerExtension._debugReportCreateInfo;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;

	//������Ҫ���õĲ����Ƶ��б�
	instanceInfo.enabledLayerCount = (uint32_t)layers.size();
	instanceInfo.ppEnabledLayerNames = layers.size() ? layers.data() : nullptr;

	//������Ҫ���õ���չ�б�
	instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.size() ? extensions.data() : nullptr;

	// pAllocator �����������������ڴ�ķ���
	VkResult res = vkCreateInstance(&instanceInfo,nullptr,&_instance);
	return res;
}

void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(_instance,nullptr);
}
