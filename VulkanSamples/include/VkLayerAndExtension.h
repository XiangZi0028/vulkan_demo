#pragma once
#include<vulkan/vulkan.h>
#include<vector>
struct  LayerProperties
{
	VkLayerProperties properties;
	std::vector<VkExtensionProperties> extensions;
};
class VulkanLayerAndExtension
{
public:
	VulkanLayerAndExtension() {};
	~VulkanLayerAndExtension(){};
	
	//instance/global layer
	VkResult getInstanceLayerProperties();

	/*ȫ����չ
	* ��ȡ����ʵ�������豸����չ�Լ����ǵ�������Ϣ
	* ����һ���Ϸ��������豸pgpu���ɻ�ȡ�����豸����չ
	* ����null�õ��ľ��ǻ���ʵ������չ
	*/
	VkResult getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu = nullptr);
	/*
	* �豸����չ
	*/
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);

	VkBool32 areLayersSupported(std::vector<const char *> &layerNames);

	VkResult  createDebugReportCallback();
	void destroyReportCallback();
public:
	//Ӧ�ó��������õ��Ĳ�Ĳ����б�
	std::vector<const char*> _appRequestedlayerNames;

	//Ӧ�ò��������õ��Ĳ����չ���б�
	std::vector<const char*> _appRequestedextensionNames;

	//��Ͷ�Ӧ����չ�б�
	std::vector<LayerProperties> _layerPropertiesList;
	
	//������ɾ������ָ�������
	PFN_vkCreateDebugReportCallbackEXT _dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT _dbgDestroyReportCallback;

	//���Ա���ص��ľ��
	VkDebugReportCallbackEXT _debugReportCallback;

	//�������Ա���Ĳ����ṹ��
	VkDebugReportCallbackCreateInfoEXT _debugReportCreateInfo = {};
	
	//�û�����ĵ����������
	static VkBool32 VKAPI_CALL debugFunction(VkFlags msgFlags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* layerPrefix,
		const char* msg,
		void* userData);
};

