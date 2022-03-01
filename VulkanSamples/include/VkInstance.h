#pragma once
#include<vulkan/vulkan.h>
#include<VkLayerAndExtension.h>
class VulkanInstance
{
public:
	VulkanInstance();
	~VulkanInstance();
	//vulkan 实例对象
	VkInstance _instance;

	//vulkan 实例相关的层和扩展
	VulkanLayerAndExtension _layerExtension;
	public:
	//创建Vulkan实例
		VkResult createInstance(std::vector<const char*>& layers,
			std::vector<const char*>& extensions,
			const char* appName);
	void destroyInstance();
};

