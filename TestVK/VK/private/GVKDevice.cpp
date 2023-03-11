#include<GVKDevice.h>

GVKDevice::GVKDevice(GVKInstance Instance)
	:mInstance(Instance)
{
	EnumerateGPUs();
	SelectTargetGPU();
}

GVKDevice::~GVKDevice()
{
}

VkPhysicalDevice GVKDevice::GetCurrentDevice() const
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

void GVKDevice::EnumerateGPUs() 
{
	uint32_t GPUCount;
	vkEnumeratePhysicalDevices(mInstance.GetVKInstance(), &GPUCount, nullptr);
	if (GPUCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
	else
	{
		mGPUs.resize(GPUCount);
		vkEnumeratePhysicalDevices(mInstance.GetVKInstance(), &GPUCount, mGPUs.data());
	}
}

void GVKDevice::SelectTargetGPU()
{
	int MaxScore = -1;
	for (uint32_t Index = 0; Index < mGPUs.size(); Index++) {
		if (auto Score = RateDeviceSuitability(&mGPUs[Index]))
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

int	GVKDevice::RateDeviceSuitability(VkPhysicalDevice* GPU) const
{
	VkPhysicalDeviceProperties DeviceProperties;
	VkPhysicalDeviceFeatures DeviceFeatures;
	if (!IsGPUSuitable(GPU, &DeviceProperties, &DeviceFeatures))
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