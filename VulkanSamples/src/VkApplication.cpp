#include <vkApplication.h>
#include<assert.h>

std::once_flag VulkanApplication::_onlyOnce;
std::unique_ptr<VulkanApplication> VulkanApplication::_Instance;

std::vector<const char*> instanceExtensionNames = { VK_KHR_SURFACE_EXTENSION_NAME,
													"VK_KHR_win32_surface",
													 VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
std::vector<const char*> layerNames = {
										"VK_LAYER_LUNARG_api_dump",
										//"VK_LAYER_KHRONOS_validation",								
};

VulkanApplication::VulkanApplication()
{
	//应用程序启动时开始枚举实列层
	this->_instanceObj._layerExtension.getInstanceLayerProperties();
}

VulkanApplication::~VulkanApplication()
{
}
VkResult VulkanApplication::createVulkanInstance(std::vector<const char*>& layers,
												std::vector<const char*>& extensions,
												const char* appName)
{
	this->_instanceObj.createInstance(layers, extensions, appName);
	return VK_SUCCESS;
}
VulkanApplication* VulkanApplication::GetInstance()
{
	std::call_once(_onlyOnce, []() {
		_Instance.reset(new VulkanApplication());});
	return _Instance.get();
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList)
{
	VkResult res;
	//记录GPU数量
	uint32_t gpuDeviceCount;
	res = vkEnumeratePhysicalDevices(_instanceObj._instance,&gpuDeviceCount,nullptr);
	assert(res == VK_SUCCESS);

	assert(gpuDeviceCount);
	//给gpuList分配空间
	gpuList.resize(gpuDeviceCount);
	//获取物理设备
	res = vkEnumeratePhysicalDevices(_instanceObj._instance, &gpuDeviceCount, gpuList.data());
	assert(res == VK_SUCCESS);
	return res;
}



VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
	//它负责管理物理和逻辑设备，也负责管理它所包含的队列和属性
	_deviceObj = new VulkanDevice(gpu);
	if (!_deviceObj)
	{
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	//打印设备端可用的层以及它们的扩展信息
	_deviceObj->_layerExtension.getDeviceExtensionProperties(gpu);
	//获取物理设备或者gpu的属性
	vkGetPhysicalDeviceProperties(*gpu, &_deviceObj->_gpuProps);
	//特性
	vkGetPhysicalDeviceFeatures(*gpu, &_deviceObj->_gpuFeatures);
	//获取物理设备或者gpu内存属性
	vkGetPhysicalDeviceMemoryProperties(*gpu, &_deviceObj->_memoryProps);
	//查询物理设备端可用的队列并返回它们的属性
	_deviceObj->getPhysicalDeviceQueueAndProperties();

	//获取能够支持图形操作流水线队列
	_deviceObj->getGraphicsQueueHandle();

	//创建逻辑设备，确保设备已经关联到一个图形队列当中了
	_deviceObj->createDevice(layers, extensions);
	return VK_SUCCESS;
}

void VulkanApplication::setInstanceExtensionNames(std::vector<const char*>& inInstanceExtensionNames)
{
	_instanceExtensionNames = inInstanceExtensionNames;
}

void VulkanApplication::setLayerNames(std::vector<const char*>& inLayerNames)
{
	_layerNames = inLayerNames;
}

bool VulkanApplication::isDeviceSuitable(VkPhysicalDevice gpu)
{
	return true;
}


VkResult VulkanApplication::initialize(int gpuIndex)
{
	char title[] = "Hello World!!!";

	if (debugFlag)
	{
		_instanceObj._layerExtension.areLayersSupported(layerNames);
	}

	//创建vulkan实例 并启用指定的层和扩展的名称
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	if (debugFlag) {
		_instanceObj._layerExtension.createDebugReportCallback();
	}

	//获取系统中物理设备的列表
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);

	/*
	* 正常情况下没有特殊需求的话这里应该先遍历这些device,然后找到符合要求的哪一个
	* for (auto gpu : gpuList)
	*{
	*	if (isDeviceSuitable(gpu))
	*	{
	*		handShakeWithDevice(&gpuList[gpuList.size() > gpuIndex ? gpuIndex : 0], _layerNames, _instanceExtensionNames);
	*		break;
	*	}
	*}
	*/
	
	//例子中只用到了第一个可用的设备
	if (gpuList.size() >= 0)
	{
		handShakeWithDevice(&gpuList[gpuList.size() > gpuIndex ? gpuIndex : 0], _layerNames, _instanceExtensionNames);
	}


	return VK_SUCCESS;
}
