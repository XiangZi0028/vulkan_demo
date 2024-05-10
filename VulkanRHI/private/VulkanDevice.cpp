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

void VulkanDevice::SetupPixelFormatMap()
{
	//查询并统计当前设备支持的Format的支持情况
	for (int32_t index = 0; index < VkFormat::VK_FORMAT_MAX_ENUM; ++index)
	{
		const VkFormat format = (VkFormat)index;
		VkFormatProperties curFormatProperties = {};
		vkGetPhysicalDeviceFormatProperties(mGpu, format, &curFormatProperties);
		if ((curFormatProperties.bufferFeatures != 0) || 
			(curFormatProperties.linearTilingFeatures != 0) || 
			(curFormatProperties.optimalTilingFeatures != 0))
		{
			VkFormatProperties& properties = mFormatProperties[format];
			properties = curFormatProperties;
		}
	}
	//初始化
	for (int32_t index = 0; index < EPixelFormat::PF_NUMMAX; ++index)
	{
		PixelFormatInfo& pixelFormatInfo = GPixelFormatsMap[EPixelFormat(index)];
		pixelFormatInfo.platformFormat = VK_FORMAT_UNDEFINED;
		pixelFormatInfo.supported = false;

		VkComponentMapping& componentMapping = GPixelFormatComponentMap[EPixelFormat(index)];
		componentMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		componentMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		componentMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		componentMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	}

	//lambda: vkformat supported
	auto IsVkFormatSupported = [](VkFormat inFormat)
	{
		if (mFormatProperties.find(inFormat) != mFormatProperties.end())
		{
			return true;
		}
		return false;
	};
	
	auto MapSupportedFormat = [&IsVkFormatSupported](EPixelFormat inPixelFormat, VkFormat inVkFormat)
	{
		PixelFormatInfo& pixelFormatInfo = VulkanDevice::GPixelFormatsMap[inPixelFormat];
		pixelFormatInfo.supported = IsVkFormatSupported(inVkFormat);
		pixelFormatInfo.platformFormat = inVkFormat;
	};
	auto FormatComponentMapping = [](EPixelFormat inPixelFormat, VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a)
	{
		VkComponentMapping& componentMapping = GPixelFormatComponentMap[inPixelFormat];
		componentMapping.r = r;
		componentMapping.g = g;
		componentMapping.b = b;
		componentMapping.a = a;
	};
	MapSupportedFormat(PF_B8G8R8A8, VK_FORMAT_B8G8R8A8_UNORM);
	FormatComponentMapping(PF_B8G8R8A8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_G8, VK_FORMAT_R8_UNORM);
	FormatComponentMapping(PF_G8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_G16, VK_FORMAT_R16_UNORM);
	FormatComponentMapping(PF_G16, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_FloatRGB, VK_FORMAT_B10G11R11_UFLOAT_PACK32);
	FormatComponentMapping(PF_FloatRGB, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_FloatRGBA, VK_FORMAT_R16G16B16A16_SFLOAT);
	FormatComponentMapping(PF_FloatRGBA, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_DepthStencil, VK_FORMAT_D32_SFLOAT_S8_UINT);
	if (!GPixelFormatsMap[PF_DepthStencil].supported)
	{
		MapSupportedFormat(PF_DepthStencil, VK_FORMAT_D24_UNORM_S8_UINT);
		if (!GPixelFormatsMap[PF_DepthStencil].supported)
		{
			MapSupportedFormat(PF_DepthStencil, VK_FORMAT_D16_UNORM_S8_UINT);
			if (!GPixelFormatsMap[PF_DepthStencil].supported) {
				//No stencil texture format supported;
			}
		}
	}
	FormatComponentMapping(PF_DepthStencil, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);

	MapSupportedFormat(PF_ShadowDepth, VK_FORMAT_D16_UNORM);
	FormatComponentMapping(PF_ShadowDepth, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);

	MapSupportedFormat(PF_G32R32F, VK_FORMAT_R32G32_SFLOAT);
	FormatComponentMapping(PF_G32R32F, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_A32B32G32R32F, VK_FORMAT_R32G32B32A32_SFLOAT);
	FormatComponentMapping(PF_A32B32G32R32F, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_G16R16, VK_FORMAT_R16G16_UNORM);
	FormatComponentMapping(PF_G16R16, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_G16R16F, VK_FORMAT_R16G16_SFLOAT);
	FormatComponentMapping(PF_G16R16F, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_G16R16F_FILTER, VK_FORMAT_R16G16_SFLOAT);
	FormatComponentMapping(PF_G16R16F_FILTER, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R16_UINT, VK_FORMAT_R16_UINT);
	FormatComponentMapping(PF_R16_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R16_SINT, VK_FORMAT_R16_SINT);
	FormatComponentMapping(PF_R16_SINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R32_UINT, VK_FORMAT_R32_UINT);
	FormatComponentMapping(PF_R32_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R32_SINT, VK_FORMAT_R32_SINT);
	FormatComponentMapping(PF_R32_SINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R8_UINT, VK_FORMAT_R8_UINT);
	FormatComponentMapping(PF_R8_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_D24, VK_FORMAT_D24_UNORM_S8_UINT);
	if (!GPixelFormatsMap[PF_D24].supported)
	{
		MapSupportedFormat(PF_D24, VK_FORMAT_D16_UNORM_S8_UINT);
		if (!GPixelFormatsMap[PF_D24].supported)
		{
			MapSupportedFormat(PF_D24, VK_FORMAT_D32_SFLOAT);
			if (!GPixelFormatsMap[PF_D24].supported)
			{
				MapSupportedFormat(PF_D24, VK_FORMAT_D32_SFLOAT_S8_UINT);
				if (!GPixelFormatsMap[PF_D24].supported)
				{
					MapSupportedFormat(PF_D24, VK_FORMAT_D16_UNORM);
					if (!GPixelFormatsMap[PF_D24].supported) {
						//No Depth texture format supported!
					}
				}
			}
		}
	}
	FormatComponentMapping(PF_D24, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R16F, VK_FORMAT_R16_SFLOAT);
	FormatComponentMapping(PF_R16F, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R16F_FILTER, VK_FORMAT_R16_SFLOAT);
	FormatComponentMapping(PF_R16F_FILTER, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_FloatR11G11B10, VK_FORMAT_B10G11R11_UFLOAT_PACK32);
	FormatComponentMapping(PF_FloatR11G11B10, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_A2B10G10R10, VK_FORMAT_A2B10G10R10_UNORM_PACK32);
	FormatComponentMapping(PF_A2B10G10R10, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_A16B16G16R16, VK_FORMAT_R16G16B16A16_UNORM);
	FormatComponentMapping(PF_A16B16G16R16, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_A8, VK_FORMAT_R8_UNORM);
	FormatComponentMapping(PF_A8, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_R);

	MapSupportedFormat(PF_R5G6B5_UNORM, VK_FORMAT_R5G6B5_UNORM_PACK16);
	FormatComponentMapping(PF_R5G6B5_UNORM, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R8G8B8A8, VK_FORMAT_R8G8B8A8_UNORM);
	FormatComponentMapping(PF_R8G8B8A8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R8G8B8A8_UINT, VK_FORMAT_R8G8B8A8_UINT);
	FormatComponentMapping(PF_R8G8B8A8_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R8G8B8A8_SNORM, VK_FORMAT_R8G8B8A8_SNORM);
	FormatComponentMapping(PF_R8G8B8A8_SNORM, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R16G16_UINT, VK_FORMAT_R16G16_UINT);
	FormatComponentMapping(PF_R16G16_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R16G16B16A16_UINT, VK_FORMAT_R16G16B16A16_UINT);
	FormatComponentMapping(PF_R16G16B16A16_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R16G16B16A16_SINT, VK_FORMAT_R16G16B16A16_SINT);
	FormatComponentMapping(PF_R16G16B16A16_SINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R32G32B32A32_UINT, VK_FORMAT_R32G32B32A32_UINT);
	FormatComponentMapping(PF_R32G32B32A32_UINT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R16G16B16A16_SNORM, VK_FORMAT_R16G16B16A16_SNORM);
	FormatComponentMapping(PF_R16G16B16A16_SNORM, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R16G16B16A16_UNORM, VK_FORMAT_R16G16B16A16_UNORM);
	FormatComponentMapping(PF_R16G16B16A16_UNORM, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	MapSupportedFormat(PF_R8G8, VK_FORMAT_R8G8_UNORM);
	FormatComponentMapping(PF_R8G8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_V8U8, VK_FORMAT_R8G8_UNORM);
	FormatComponentMapping(PF_V8U8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);

	MapSupportedFormat(PF_R32_FLOAT, VK_FORMAT_R32_SFLOAT);
	FormatComponentMapping(PF_R32_FLOAT, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO);
}

void VulkanDevice::InitGPU()
{
	CreateDevice();
	//建立映射关系
	SetupPixelFormatMap();
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
		return (it->second);
	}
	else
	{
		VkFormatProperties newVkFormatProperties = {};
		vkGetPhysicalDeviceFormatProperties(mGpu, inFormat, &newVkFormatProperties);
		mFormatProperties[inFormat] = newVkFormatProperties;
		return newVkFormatProperties;
	}
}