#pragma once
#include<vulkan/vulkan.h>
#include<VkLayerAndExtension.h>
class VulkanInstance
{
public:
	VulkanInstance();
	~VulkanInstance();
	//vulkan ʵ������
	VkInstance _instance;

	//vulkan ʵ����صĲ����չ
	VulkanLayerAndExtension _layerExtension;
	public:
	//����Vulkanʵ��
		VkResult createInstance(std::vector<const char*>& layers,
			std::vector<const char*>& extensions,
			const char* appName);
	void destroyInstance();
};

