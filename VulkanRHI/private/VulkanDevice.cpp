#include"VulkanRHI/public/VulkanDevice.h"
#include"VulkanRHI/public/VulkanCommandBuffer.h"
VulkanDevice::EGpuType VulkanDevice::QueryGPUType()
{
	if (mGpuType != EGpuType::Unknown)
	{
		return mGpuType;
	}
	bool discrete = false;
	vkGetPhysicalDeviceProperties(mGpu, &mGpuProperties);
	switch (mGpuProperties.deviceType)
	{
	case  VK_PHYSICAL_DEVICE_TYPE_OTHER:
		mGpuType = EGpuType::Other;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		mGpuType = EGpuType::Integrate;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		mGpuType = EGpuType::Discrete;
		discrete = true;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		mGpuType  = EGpuType::Virtual;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		mGpuType = 	EGpuType::Unknown;
		break;
	default:
		break;
	}
	unsigned int queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mGpu, &queueFamilyCount, nullptr);
	mQueueFamliyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(mGpu, &queueFamilyCount, mQueueFamliyProperties.data());
	return mGpuType;
}

void VulkanDevice::InitGPU()
{
	CreateDevice();
}

void VulkanDevice::CreateDevice()
{
	vkGetPhysicalDeviceFeatures(mGpu, &mGpuFeature);

	//枚举Device在所有Layer
	unsigned int count = 0;
	vkEnumerateDeviceLayerProperties(mGpu, &count, nullptr);
	std::vector<VkLayerProperties> GpuLayerProperties(count);
	vkEnumerateDeviceLayerProperties(mGpu, &count, GpuLayerProperties.data());

	TArray(TArray(VkExtensionProperties)) DeviceLayerExtensions;
	DeviceLayerExtensions.resize(count);

	auto enumerateDeviceLayerExtensionProperties = [](VkPhysicalDevice device, const char* layerName, TArray(VkExtensionProperties)& layerExtensions)
	{
		unsigned int count = 0;
		vkEnumerateDeviceExtensionProperties(device, layerName, &count, nullptr);
		layerExtensions.resize(count);
		vkEnumerateDeviceExtensionProperties(device, layerName, &count, layerExtensions.data());
	};
	TArray(const char*) allLayerNames;
	for (unsigned int Index = 0; Index < count; Index++)
	{
		allLayerNames.push_back(GpuLayerProperties[Index].layerName);
		enumerateDeviceLayerExtensionProperties(mGpu, GpuLayerProperties[Index].layerName, DeviceLayerExtensions[Index]);
	}

	//搜集所有需要在Extensions
	TArray(const char*) allNeededExtensions;
	for (auto layerExtension : DeviceLayerExtensions)
	{
		for (auto extension : layerExtension)
		{
			bool bExtensionExist = false;
			for (auto existExtension : allNeededExtensions)
			{
				if (strcmp(existExtension, extension.extensionName) == 0)
				{
					bExtensionExist = true;
					break;
				}
			}
			if (!bExtensionExist)
			{
				allNeededExtensions.push_back(extension.extensionName);
			}
		}
	}
	//获取平台需要在DeviceExtension，添加到All中

	//搜集一些全局在DeviceExtension
	{
		allNeededExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		allNeededExtensions.push_back(VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME);
		allNeededExtensions.push_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);
	}


	VkDeviceCreateInfo deviceCreateInfo;
	ZeroVulkanStruct(deviceCreateInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
	deviceCreateInfo.enabledExtensionCount = uint32_t(allNeededExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = allNeededExtensions.data();
	deviceCreateInfo.enabledLayerCount = uint32_t(allLayerNames.size());
	deviceCreateInfo.ppEnabledLayerNames = allLayerNames.data();
	deviceCreateInfo.pEnabledFeatures = &mGpuFeature;

	//根据QueueFamily
	TArray(VkDeviceQueueCreateInfo) queueFamilyCreateInfos;
	TArray(float) queuePriorities; 
	queuePriorities.resize(100,1);

	for (int Index = 0; Index < mQueueFamliyProperties.size(); Index++)
	{
		bool bQueueHasUsage = false;
		auto& queueFamliyProperties = mQueueFamliyProperties[Index];
		if (queueFamliyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			mQueueFamilyIndices.GraphicsFamily = Index;
			mQueueFamilyIndices.PresentFamily = Index;
			bQueueHasUsage = true;
		}
		if (queueFamliyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			mQueueFamilyIndices.ComputeFamily = Index;
			bQueueHasUsage = true;
		}
		if (queueFamliyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			mQueueFamilyIndices.TransferFamily = Index;
			bQueueHasUsage = true;
		}


		if (!mQueueFamilyIndices.PresentFamily.has_value())
		{
			VkBool32 supportsPresent = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(mGpu, Index, mSurface, &supportsPresent);
			mQueueFamilyIndices.PresentFamily = Index;
		}

		if (bQueueHasUsage)
		{
			VkDeviceQueueCreateInfo queueCreateInfo;
			//queueCreateInfo.flags = VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT;
			queueCreateInfo.queueFamilyIndex = Index;
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.pNext = nullptr;
			queueCreateInfo.queueCount = queueFamliyProperties.queueCount;
			queueCreateInfo.flags = 0;
			//float& Priority = queuePriorities[queuePriorities.size()-1];
			//Priority += Index / queueFamliyProperties.queueCount;
			queueCreateInfo.pQueuePriorities = queuePriorities.data();//这里我默认Queue的数量越多优先级越高
			queueFamilyCreateInfos.push_back(queueCreateInfo);
		}
	}
	deviceCreateInfo.pQueueCreateInfos = queueFamilyCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = uint32_t(queueFamilyCreateInfos.size());
	deviceCreateInfo.flags = 0;
	VkResult result = vkCreateDevice(mGpu, &deviceCreateInfo, nullptr, &mDevice);

	if (result == VK_ERROR_INITIALIZATION_FAILED)
	{
		throw std::runtime_error("failed to create logic device");
	}
	//获取刚刚创建的Queue
	shared_ptr<VulkanQueue> GraphicQueue = VulkanQueue::CreateQueue(this->shared_from_this(), mQueueFamilyIndices.GraphicsFamily.value());
	shared_ptr<VulkanQueue> ComputeQueue = VulkanQueue::CreateQueue(this->shared_from_this(), mQueueFamilyIndices.ComputeFamily.value());
	shared_ptr<VulkanQueue> TransferQueue = VulkanQueue::CreateQueue(this->shared_from_this(), mQueueFamilyIndices.TransferFamily.value());
	shared_ptr<VulkanQueue> PresentQueue = VulkanQueue::CreateQueue(this->shared_from_this(), mQueueFamilyIndices.PresentFamily.value());
	mQueues.GraphicsQueue = GraphicQueue;
	mQueues.ComputeQueue = ComputeQueue;
	mQueues.TransferQueue = TransferQueue;
	mQueues.PresentQueue = PresentQueue;
}


const VkFormatProperties& VulkanDevice::GetPhysicalDeviceFormatProperties(VkFormat inFormat)
{
	auto it = mFormatProperties.find(inFormat);
	if (it != mFormatProperties.end())
	{
		return *(it->second);
	}
	else
	{
		VkFormatProperties* newVkFormatProperties = new VkFormatProperties();
		vkGetPhysicalDeviceFormatProperties(mGpu, inFormat, newVkFormatProperties);
		mFormatProperties[inFormat] = newVkFormatProperties;
		return *(newVkFormatProperties);
	}
}