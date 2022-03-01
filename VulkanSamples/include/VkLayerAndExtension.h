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

	/*全局扩展
	* 获取基于实例活着设备的扩展以及他们的属性信息
	* 传入一个合法的物流设备pgpu即可获取基于设备的扩展
	* 传入null得到的就是基于实例的扩展
	*/
	VkResult getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu = nullptr);
	/*
	* 设备的扩展
	*/
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);

	VkBool32 areLayersSupported(std::vector<const char *> &layerNames);

	VkResult  createDebugReportCallback();
	void destroyReportCallback();
public:
	//应用程序所需用到的层的层名列表
	std::vector<const char*> _appRequestedlayerNames;

	//应用层序所需用到的层的扩展名列表
	std::vector<const char*> _appRequestedextensionNames;

	//层和对应的扩展列表
	std::vector<LayerProperties> _layerPropertiesList;
	
	//创建和删除函数指针的声明
	PFN_vkCreateDebugReportCallbackEXT _dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT _dbgDestroyReportCallback;

	//调试报告回调的句柄
	VkDebugReportCallbackEXT _debugReportCallback;

	//创建调试报告的参数结构体
	VkDebugReportCallbackCreateInfoEXT _debugReportCreateInfo = {};
	
	//用户定义的调试输出函数
	static VkBool32 VKAPI_CALL debugFunction(VkFlags msgFlags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* layerPrefix,
		const char* msg,
		void* userData);
};

