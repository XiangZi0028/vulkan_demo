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
	//Ӧ�ó�������ʱ��ʼö��ʵ�в�
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
	//��¼GPU����
	uint32_t gpuDeviceCount;
	res = vkEnumeratePhysicalDevices(_instanceObj._instance,&gpuDeviceCount,nullptr);
	assert(res == VK_SUCCESS);

	assert(gpuDeviceCount);
	//��gpuList����ռ�
	gpuList.resize(gpuDeviceCount);
	//��ȡ�����豸
	res = vkEnumeratePhysicalDevices(_instanceObj._instance, &gpuDeviceCount, gpuList.data());
	assert(res == VK_SUCCESS);
	return res;
}



VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
	//���������������߼��豸��Ҳ����������������Ķ��к�����
	_deviceObj = new VulkanDevice(gpu);
	if (!_deviceObj)
	{
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	//��ӡ�豸�˿��õĲ��Լ����ǵ���չ��Ϣ
	_deviceObj->_layerExtension.getDeviceExtensionProperties(gpu);
	//��ȡ�����豸����gpu������
	vkGetPhysicalDeviceProperties(*gpu, &_deviceObj->_gpuProps);
	//����
	vkGetPhysicalDeviceFeatures(*gpu, &_deviceObj->_gpuFeatures);
	//��ȡ�����豸����gpu�ڴ�����
	vkGetPhysicalDeviceMemoryProperties(*gpu, &_deviceObj->_memoryProps);
	//��ѯ�����豸�˿��õĶ��в��������ǵ�����
	_deviceObj->getPhysicalDeviceQueueAndProperties();

	//��ȡ�ܹ�֧��ͼ�β�����ˮ�߶���
	_deviceObj->getGraphicsQueueHandle();

	//�����߼��豸��ȷ���豸�Ѿ�������һ��ͼ�ζ��е�����
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

	//����vulkanʵ�� ������ָ���Ĳ����չ������
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	if (debugFlag) {
		_instanceObj._layerExtension.createDebugReportCallback();
	}

	//��ȡϵͳ�������豸���б�
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);

	/*
	* ���������û����������Ļ�����Ӧ���ȱ�����Щdevice,Ȼ���ҵ�����Ҫ�����һ��
	* for (auto gpu : gpuList)
	*{
	*	if (isDeviceSuitable(gpu))
	*	{
	*		handShakeWithDevice(&gpuList[gpuList.size() > gpuIndex ? gpuIndex : 0], _layerNames, _instanceExtensionNames);
	*		break;
	*	}
	*}
	*/
	
	//������ֻ�õ��˵�һ�����õ��豸
	if (gpuList.size() >= 0)
	{
		handShakeWithDevice(&gpuList[gpuList.size() > gpuIndex ? gpuIndex : 0], _layerNames, _instanceExtensionNames);
	}


	return VK_SUCCESS;
}
