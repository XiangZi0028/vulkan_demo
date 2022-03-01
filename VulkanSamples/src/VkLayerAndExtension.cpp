#include <VkLayerAndExtension.h>
#include<VkApplication.h>
#include<iostream>



VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu)
{
	//保存每一层的扩展总数
	uint32_t extensionCount;
	VkResult result;
	char* layerName = layerProps.properties.layerName;
	do 
	{
		//获取当前层中的扩展的总数
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
		//获取所有的扩展属性
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
	//用来检查Vukan API 结果状态的变量
	VkResult res=VK_INCOMPLETE;
	//查询各个层的所有扩展并且给予保存
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
	//删除不支持的
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
	
	//获取vkCreateDebugReportCallbackEXT函数
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

	//定义调试报告的结构体，并且设置“debugFunction”，该函数用来打印调试信息到控制台
	_debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	_debugReportCreateInfo.pNext = nullptr;
	_debugReportCreateInfo.pfnCallback = debugFunction;
	_debugReportCreateInfo.pUserData = nullptr;
	_debugReportCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
									VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
									VK_DEBUG_REPORT_ERROR_BIT_EXT |
									VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	//创建调试报告回调函数 并且将句柄存储到‘_debugReportCallback’
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
	//存储实例层的数目
	uint32_t instanceLayerCount;
	//通过vector层来存储层的属性
	std::vector<VkLayerProperties> layerProperties;

	//检查vulkan api 的执行结果
	VkResult result;
	
	//查询所有层 将层的属性存储到layerProperties中
	do 
	{
		//如果pPropertis填NULL 则函数会返回一个总记的层数到instanceLayerCount
		//如果pProperties填的是一个数组 返回层的属性信息
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount,nullptr);
		if (result)
			return result;
		if (instanceLayerCount == 0)
			return VK_INCOMPLETE;//获取失败
		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result==VK_INCOMPLETE);

	//查询每个层的所有扩展信息，并进行保存

	std::cout << "Instance Layer:" << std::endl;
	std::cout << "=================" << std::endl;
	for (auto globalLayerProp : layerProperties)
	{
		//显示层的名字和描述信息
		std::cout << "\n" << globalLayerProp.description
					<< "\n\t|\n\y---[layer Name]-->" 
					<< globalLayerProp.layerName << std::endl;
		LayerProperties layerprops;
		layerprops.properties = globalLayerProp;

		//根据层属性信息，获取基于实例的扩展
		result = getExtensionProperties(layerprops);
		if(result)
			continue;
		_layerPropertiesList.push_back(layerprops);
		//显示每个实例层的扩展名称
		for (auto j : layerprops.extensions)
		{
			std::cout << "\n\t|\n\t---[layer Extension]-->"
						<< j.extensionName << std::endl;
		}

	}
	return result;
}
