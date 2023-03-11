#pragma once
#include<vulkan/vulkan.h>
#include<VkLayerAndExtension.h>
#include<iostream>
/*
* 负责管理物理设备(VkPhysicalDevice)以及逻辑设备(VkDevice)
* 此外它也负责管理物理设备中的队列簇
*/
class VulkanDevice
{
public:
	VulkanDevice(VkPhysicalDevice *gpu);
	~VulkanDevice();
	
	//创建逻辑设备
	VkResult createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions);
	void memoryTypeFromProperties();
	void destroyDevice();
	uint32_t getGraphicsQueueHandle();
	//uint32_t getComputeQueueHandle();
	//uint32_t getTansformQueueHandle();
	void getDeviceQueue();
	void getPhysicalDeviceQueueAndProperties();
	void initializeDeviceQueue();
public:
	//设备相关的成员变量
	VkDevice _device;//逻辑设备
	VkPhysicalDevice* _gpu;//物理设备
	VkPhysicalDeviceProperties _gpuProps;//物理设备属性
	VkPhysicalDeviceMemoryProperties _memoryProps;//
	VkPhysicalDeviceFeatures _gpuFeatures;

	//队列相关属性
	VkQueue _queue;//vk 队列对象
	std::vector<VkQueueFamilyProperties> _queueFamilyProps;//保存物理设备暴露的所有队列
	uint32_t _graphicsQueueFamilyIndex;//保存图形队列的索引号
	uint32_t _graphicsQueueWithPresentIndex;
	uint32_t _queueFamilyCount;//设备中的队列簇的数量
	VulkanLayerAndExtension _layerExtension;//设备相关的扩展
};

