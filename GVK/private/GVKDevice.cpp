#include<GVKDevice.h>
#include<set>
GVKDevice::GVKDevice(GVKInstance* Instance)
	:mInstance(Instance)
{
	EnumerateGPUs();
	InitRequiredDeviceExtensions();
	SelectTargetGPU();
	mQueue = new GVKQueue(this);
	CreateLogicalDevice();
	mQueue->CreateDeviceQueue();
}

GVKDevice::~GVKDevice()
{
}

VkPhysicalDevice GVKDevice::GetCurrentGPU() const
{
	if (mGPUs.size() == 0 || mCurrentGPUIndex == -1 || mCurrentGPUIndex >= mGPUs.size())
	{
		return VK_NULL_HANDLE;
	}
	else
	{
		return mGPUs[mCurrentGPUIndex];
	}
}

VkDevice GVKDevice::GetVKDevice() const
{
	return mDevice;
}

void GVKDevice::EnumerateGPUs() 
{
	uint32_t GPUCount;
	vkEnumeratePhysicalDevices(mInstance->GetVKInstance(), &GPUCount, nullptr);
	if (GPUCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
	else
	{
		mGPUs.resize(GPUCount);
		vkEnumeratePhysicalDevices(mInstance->GetVKInstance(), &GPUCount, mGPUs.data());
	}
	for (int Index = 0; Index < mGPUs.size(); Index++)
	{
		pair<int, std::vector<VkExtensionProperties>> GPUExtension;
		GPUExtension.first = Index;
		mAvailableDeviceExtensions_GPUs.push_back(GPUExtension);
		InitAvailableDeviceExtensions(Index);
	}
}

void GVKDevice::SelectTargetGPU()
{
	int MaxScore = -1;
	for (uint32_t Index = 0; Index < mGPUs.size(); Index++) {
		if (auto Score = RateDeviceSuitability(Index))
		{
			if (Score > MaxScore)
			{
				mCurrentGPUIndex = Index;
				MaxScore = Score;
			}
		}
	}
	if (mCurrentGPUIndex == -1) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool GVKDevice::IsGPUSuitable(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties* DeviceProperties, VkPhysicalDeviceFeatures* DeviceFeatures) const
{
	
	GetGPUPropertiseAndFeatures(GPU, *DeviceProperties, *DeviceFeatures);

	return DeviceProperties->deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}
void GVKDevice::GetGPUPropertiseAndFeatures(const VkPhysicalDevice* GPU, VkPhysicalDeviceProperties &DeviceProperties, VkPhysicalDeviceFeatures & DeviceFeatures) const
{
	vkGetPhysicalDeviceProperties(*GPU, &DeviceProperties);
	vkGetPhysicalDeviceFeatures(*GPU, &DeviceFeatures);
}

int	GVKDevice::RateDeviceSuitability(int GPUIndex) const
{
	VkPhysicalDeviceProperties DeviceProperties;
	VkPhysicalDeviceFeatures DeviceFeatures;
	auto Details = this->mInstance->mGVKSurfaceKHR->GetPhysicalDeviceSurfaceSupportInfos(mGPUs[GPUIndex]);
	//如果gpu surface不支持任何格式的像素或者显示模型的话就是个废物。
	if (Details->mFormats.size() == 0 || Details->mPresentModes.size() == 0)
	{
		return 0;
	}
	if (!IsGPUSuitable(&mGPUs[GPUIndex], &DeviceProperties, &DeviceFeatures) || !CheckDeviceExtensionSupport(GPUIndex))
	{
		return 0;
	}
	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += DeviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (DeviceFeatures.geometryShader) {
		score += 100;
	}

	return score;
}


void GVKDevice::CreateLogicalDevice()
{
	
	BuildVkDeviceQueueCreateInfo();
	VkDeviceCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateInfo.pQueueCreateInfos = mQueueCreateInfos.size() > 0 ? mQueueCreateInfos.data() : nullptr;
	CreateInfo.queueCreateInfoCount = static_cast<uint32_t>(mQueueCreateInfos.size());
	CreateInfo.pEnabledFeatures = &mGPUFeatures;
	if (mInstance->bEnableExtensions)
	{
			CreateInfo.enabledExtensionCount = mRequiredDeviceExtensions.size();
			CreateInfo.ppEnabledExtensionNames = mRequiredDeviceExtensions.data();

	}
	else
	{
		CreateInfo.enabledExtensionCount = 0;
		CreateInfo.ppEnabledExtensionNames = nullptr;
	}
	if (mInstance->bEnableValidationLayers)
	{
		CreateInfo.enabledLayerCount = mInstance->GetValidationLayers().size();
		CreateInfo.ppEnabledLayerNames = mInstance->GetValidationLayers().data();
	}
	else
	{
		CreateInfo.enabledLayerCount = 0;
		CreateInfo.ppEnabledLayerNames = nullptr;
	}
	if (vkCreateDevice(GetCurrentGPU(), &CreateInfo, nullptr, &mDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("Faild to Creat logical device!");
	}
	
}

void GVKDevice::BuildVkDeviceQueueCreateInfo()
{
	std::set<uint32_t> uniqueQueueFamilies = { mQueue->GetQueueFamilyIndices().PresentFamily.value(), mQueue->GetQueueFamilyIndices().GraphicsFamily.value() };
	for (auto QueueFamilyIndex : uniqueQueueFamilies)
	{
		float& CurrentQueuePriority = mQueuePrioritys.emplace_back(1.0f);
		VkDeviceQueueCreateInfo QueueCreateInfo{};
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.queueFamilyIndex = QueueFamilyIndex;
		QueueCreateInfo.queueCount = 1;
		QueueCreateInfo.pQueuePriorities = &CurrentQueuePriority;
		mQueueCreateInfos.push_back(QueueCreateInfo);
	}
	
}

void GVKDevice::Cleanup()
{
	vkDestroyDevice(mDevice,nullptr);
}

void GVKDevice::InitAvailableDeviceExtensions(int Index)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(mGPUs[Index] ,nullptr, &extensionCount, nullptr);

	mAvailableDeviceExtensions_GPUs[Index].second.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(mGPUs[Index], nullptr, &extensionCount, mAvailableDeviceExtensions_GPUs[Index].second.data());
}

void GVKDevice::InitRequiredDeviceExtensions()
{
	mRequiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

bool GVKDevice::CheckDeviceExtensionSupport(int GPUIndex) const
{
	auto GPUExtension = mAvailableDeviceExtensions_GPUs[GPUIndex];
	for (auto RequiredExtension : mRequiredDeviceExtensions)
	{
		bool bFoundExtension = false;
		for (auto Extension : GPUExtension.second)
		{
			if (strcmp(Extension.extensionName, RequiredExtension))
			{
				bFoundExtension = true;
			}
		}
		if (!bFoundExtension)
		{
			//throw std::runtime_error("Extension was requeired,but Device(GPU) don't support!");
			std::cout << "GPU[" << GPUIndex << "]: Extension was requeired,but Device(GPU) don't support!" << endl;
			break;
			return bFoundExtension;
		}
	}
	return true;
}