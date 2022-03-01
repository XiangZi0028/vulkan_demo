#include <VkLayerAndExtension.h>
#include<VkApplication.h>
#include<iostream>



VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu)
{
	//����ÿһ�����չ����
	uint32_t extensionCount;
	VkResult result;
	char* layerName = layerProps.properties.layerName;
	do 
	{
		//��ȡ��ǰ���е���չ������
		if (gpu)
		{
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, nullptr);
		}
		else
		{
			result = vkEnumerateInstanceExtensionProperties(layerName,&extensionCount,nullptr);
		}
		if (result || extensionCount == 0)
			continue;
		layerProps.extensions.resize(extensionCount);
		//��ȡ���е���չ����
		if (gpu)
		{
			result = vkEnumerateDeviceExtensionProperties(*gpu,layerName,&extensionCount,layerProps.extensions.data());
		}
		else
		{
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
		}
	} while (result==VK_INCOMPLETE);
	return result;
}

VkResult VulkanLayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu)
{
	//�������Vukan API ���״̬�ı���
	VkResult res=VK_INCOMPLETE;
	//��ѯ�������������չ���Ҹ��豣��
	std::cout << "Device Extensions" << std::endl;
	std::cout << "====================" << std::endl;
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	std::vector<LayerProperties>* instanceLayerProp = &appObj->GetInstance()->_instanceObj._layerExtension._layerPropertiesList;
	for (auto globalLayerProp : *instanceLayerProp)
	{
		LayerProperties layerProps;
		layerProps.properties = globalLayerProp.properties;
		if (res = getExtensionProperties(layerProps, gpu))
				continue;
		_layerPropertiesList.push_back(layerProps);


		if (layerProps.extensions.size())
		{
			for (auto j:layerProps.extensions)
			{
				std::cout << "\t\t|\n\t\t|--[Device Extension]-->"
					<< j.extensionName << "\n";
			}
		}
		else
			std::cout << "\t\t|\n\t\t|---[Device Extension]--> No extension found \n";

	}	
	return res;
}

VkBool32 VulkanLayerAndExtension::areLayersSupported(std::vector<const char*>& layerNames)
{
	uint32_t checkoutCount = layerNames.size();
	uint32_t layerCount = _layerPropertiesList.size();
	std::vector<const char*> unsupportLayerNames;
	for (uint32_t i = 0; i < checkoutCount; i++)
	{
		VkBool32 isSupported = false;
		for (uint32_t j = 0; j < layerCount; j++)
		{
			if (!strcmp(layerNames[i], _layerPropertiesList[j].properties.layerName))
				isSupported = true;
		}
		if (!isSupported)
		{
			std::cout << "No Layer support found,removed from layer :"
				<< layerNames[i] << std:: endl;
			unsupportLayerNames.push_back(layerNames[i]);
		}
		else
		{
			std::cout << "Layer Support :" << layerNames[i] << std::endl;
		}
	}
	//ɾ����֧�ֵ�
	for (auto i : unsupportLayerNames)
	{
		auto it = std::find(layerNames.begin(), layerNames.end(), i);
		if (it != layerNames.end())
		{
			layerNames.erase(it);
		}
	}
	return true;
}

VkResult VulkanLayerAndExtension::createDebugReportCallback()
{
	VkResult res;
	VulkanApplication* appObj=VulkanApplication::GetInstance();
	VkInstance* instance= &(appObj->_instanceObj._instance);
	
	//��ȡvkCreateDebugReportCallbackEXT����
	_dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance,"vkCreateDebugReportCallbackEXT");
	if (!_dbgCreateDebugReportCallback)
	{
		std::cout << "Error:GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function!" << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	_dbgDestroyReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugerReportCallbackEXT");
	if (!_dbgDestroyReportCallback)
	{
		std::cout << "Error:GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function!" << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	//������Ա���Ľṹ�壬�������á�debugFunction�����ú���������ӡ������Ϣ������̨
	_debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	_debugReportCreateInfo.pNext = nullptr;
	_debugReportCreateInfo.pfnCallback = debugFunction;
	_debugReportCreateInfo.pUserData = nullptr;
	_debugReportCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
									VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
									VK_DEBUG_REPORT_ERROR_BIT_EXT |
									VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	//�������Ա���ص����� ���ҽ�����洢����_debugReportCallback��
	res = _dbgCreateDebugReportCallback(*instance,&_debugReportCreateInfo,nullptr,&_debugReportCallback);
	if (res==VK_SUCCESS)
	{
		std::cout << "Debug report callback object created successfully\n" << std::endl;
	}
	return res;
}

void VulkanLayerAndExtension::destroyReportCallback()
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	_dbgDestroyReportCallback(appObj->_instanceObj._instance,_debugReportCallback,nullptr);
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLayerAndExtension::debugFunction(VkFlags msgFlags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject,
	size_t location,
	int32_t msgCode,
	const char* layerPrefix,
	const char* msg,
	void* userData)
{
	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] ERROR: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] WARNING: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] INFORMATION: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] PERFORMANCE: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] DEBUG: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else {
		return VK_FALSE;
	}

	fflush(stdout);
	return VK_TRUE;
}

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	//�洢ʵ�������Ŀ
	uint32_t instanceLayerCount;
	//ͨ��vector�����洢�������
	std::vector<VkLayerProperties> layerProperties;

	//���vulkan api ��ִ�н��
	VkResult result;
	
	//��ѯ���в� ��������Դ洢��layerProperties��
	do 
	{
		//���pPropertis��NULL �����᷵��һ���ܼǵĲ�����instanceLayerCount
		//���pProperties�����һ������ ���ز��������Ϣ
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount,nullptr);
		if (result)
			return result;
		if (instanceLayerCount == 0)
			return VK_INCOMPLETE;//��ȡʧ��
		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result==VK_INCOMPLETE);

	//��ѯÿ�����������չ��Ϣ�������б���

	std::cout << "Instance Layer:" << std::endl;
	std::cout << "=================" << std::endl;
	for (auto globalLayerProp : layerProperties)
	{
		//��ʾ������ֺ�������Ϣ
		std::cout << "\n" << globalLayerProp.description
					<< "\n\t|\n\y---[layer Name]-->" 
					<< globalLayerProp.layerName << std::endl;
		LayerProperties layerprops;
		layerprops.properties = globalLayerProp;

		//���ݲ�������Ϣ����ȡ����ʵ������չ
		result = getExtensionProperties(layerprops);
		if(result)
			continue;
		_layerPropertiesList.push_back(layerprops);
		//��ʾÿ��ʵ�������չ����
		for (auto j : layerprops.extensions)
		{
			std::cout << "\n\t|\n\t---[layer Extension]-->"
						<< j.extensionName << std::endl;
		}

	}
	return result;
}
