#include<VKDevice.h>
#include<assert.h>

VulkanDevice::VulkanDevice(VkPhysicalDevice* gpu)
{
	_gpu = gpu;
}

VulkanDevice::~VulkanDevice()
{
}

VkResult VulkanDevice::createDevice(std::vector<const char *>&layers,std::vector<const char*>& extensions)
{
	VkResult res;
	float queuePriorities[1] = { 0.0 };
	//创建对象信息
	VkDeviceQueueCreateInfo queueInfo = {};
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = nullptr;
	queueInfo.queueCount = 1;
	queueInfo.queueFamilyIndex = _graphicsQueueFamilyIndex;
	//这个参数是一个归一化之后的浮点数值的数组，它设置了用户对创建后的每个对垒的工作优先级的要求
	queueInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = nullptr;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueInfo;
	//设备的层信息属性已经被废弃
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;

	deviceInfo.enabledExtensionCount = extensions.size();
	deviceInfo.ppEnabledExtensionNames = extensions.data();
	deviceInfo.pEnabledFeatures = nullptr;

	res = vkCreateDevice(*_gpu,&deviceInfo,nullptr,&_device);
	
	assert(res == VK_SUCCESS);
	return res;
}

void VulkanDevice::memoryTypeFromProperties()
{
}

void VulkanDevice::destroyDevice()
{
}

uint32_t VulkanDevice::getGraphicsQueueHandle()
{
	bool found=false;
	//从物理设备支持的队列族中查找图形队列的队列族属性
	for (unsigned int i = 0; i < _queueFamilyCount; i++)
	{
		if (_queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			found = true;
			_graphicsQueueFamilyIndex = i;
			break;
		}
	}
	return found;
}

void VulkanDevice::initializeDeviceQueue()
{
}

void VulkanDevice::getDeviceQueue()
{
	vkGetDeviceQueue(_device, _graphicsQueueFamilyIndex,0,&_queue);
}

void VulkanDevice::getPhysicalDeviceQueueAndProperties()
{
	vkGetPhysicalDeviceQueueFamilyProperties(*_gpu,&_queueFamilyCount,nullptr);
	_queueFamilyProps.resize(_queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(*_gpu, &_queueFamilyCount, _queueFamilyProps.data() );
}
