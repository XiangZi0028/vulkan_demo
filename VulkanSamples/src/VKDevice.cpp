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
	//����������Ϣ
	VkDeviceQueueCreateInfo queueInfo = {};
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = nullptr;
	queueInfo.queueCount = 1;
	queueInfo.queueFamilyIndex = _graphicsQueueFamilyIndex;
	//���������һ����һ��֮��ĸ�����ֵ�����飬���������û��Դ������ÿ�����ݵĹ������ȼ���Ҫ��
	queueInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = nullptr;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueInfo;
	//�豸�Ĳ���Ϣ�����Ѿ�������
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
	//�������豸֧�ֵĶ������в���ͼ�ζ��еĶ���������
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
