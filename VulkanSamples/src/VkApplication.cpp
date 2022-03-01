#include <vkApplication.h>
#include<assert.h>

std::once_flag VulkanApplication::_onlyOnce;
std::unique_ptr<VulkanApplication> VulkanApplication::_Instance;

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
	//用户定义了Vulkan的高级封装函数
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

