#pragma once
#include<vulkan/vulkan.h>
#include<VkLayerAndExtension.h>
#include<iostream>
/*
* ������������豸(VkPhysicalDevice)�Լ��߼��豸(VkDevice)
* ������Ҳ������������豸�еĶ��д�
*/
class VulkanDevice
{
public:
	VulkanDevice(VkPhysicalDevice *gpu);
	~VulkanDevice();
	
	//�����߼��豸
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
	//�豸��صĳ�Ա����
	VkDevice _device;//�߼��豸
	VkPhysicalDevice* _gpu;//�����豸
	VkPhysicalDeviceProperties _gpuProps;//�����豸����
	VkPhysicalDeviceMemoryProperties _memoryProps;//
	VkPhysicalDeviceFeatures _gpuFeatures;

	//�����������
	VkQueue _queue;//vk ���ж���
	std::vector<VkQueueFamilyProperties> _queueFamilyProps;//���������豸��¶�����ж���
	uint32_t _graphicsQueueFamilyIndex;//����ͼ�ζ��е�������
	uint32_t _graphicsQueueWithPresentIndex;
	uint32_t _queueFamilyCount;//�豸�еĶ��дص�����
	VulkanLayerAndExtension _layerExtension;//�豸��ص���չ
};

