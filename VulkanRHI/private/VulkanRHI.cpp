#include "VulkanRHI/public/VulkanRHI.h"
#include "VulkanRHI/public/VulkanDevice.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "VulkanRHI/public/VulkanSwapChain.h"
void VulkanCore::InitVKCore(uint32_t inWindowWith, uint32_t inWindowHeight, bool inEnableSurface)
{
	CreateInstance();
	CreateSurface();
	SelecteGPUAndCreateDevice();
	CreateSwapChain();
}
// Begin Create VK Instance
void VulkanCore::CreateInstance()
{
	InitInstanceValidationLayers();
	InitRequiredInstanceExtensions();
	EnumerateInstanceExtensionProperties();
	EnumerateInstanceLayerProperties();
	CheckValidationLayersSupport();
	CheckExtensionsSupports();

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "ZZX";
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = uint32_t(mInstanceExtensions.size());
	instanceInfo.ppEnabledExtensionNames = mInstanceExtensions.data();
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
	instanceInfo.ppEnabledLayerNames = mValidationLayers.size() == 0 ? nullptr : mValidationLayers.data();
	if (vkCreateInstance(&instanceInfo, nullptr, &mVkInstance) != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vkinstance!");
	}
}
void VulkanCore::InitInstanceValidationLayers()
{
	//目前不支持的原因是因为我移动了VULKAN 的SDK位置，如果想放在自己的位置的需要配置
	//VulkanSDK\1.3.231.1\Bin\vkconfig.exe
	//不知道源码编译Vulkan的话支持不支持
	mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
}

void VulkanCore::InitRequiredInstanceExtensions()
{
	unsigned int glfwRequiredExtensionCount = 0;
	const char** glfwRequiredInstanceExtension = glfwGetRequiredInstanceExtensions(&glfwRequiredExtensionCount);
	//VK_KHR_SWAPCHAIN_EXTENSION_NAME 是设备扩展
	for (unsigned int Index = 0; Index < glfwRequiredExtensionCount; Index++)
	{
		mInstanceExtensions.push_back(((*glfwRequiredInstanceExtension) + Index));
	}
	//创建SurfaceKHR需要
	mInstanceExtensions.push_back("VK_KHR_win32_surface");
	//PresentMode 和 format需要
	mInstanceExtensions.push_back("VK_KHR_surface");
}

void VulkanCore::EnumerateInstanceExtensionProperties()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	mVKInstanceExtensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, mVKInstanceExtensions.data());

	std::cout << "available extensions:\n";
	for (const auto& extension : mVKInstanceExtensions) {
		std::cout << '\t' << extension.extensionName << '\n';
	}
}

void VulkanCore::EnumerateInstanceLayerProperties()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	mVKLayerProperties.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, mVKLayerProperties.data());
	std::cout << "available Layers:\n";
	for (const auto& availableLayer : mVKLayerProperties) {
		std::cout << '\t' << availableLayer.layerName << '\n';
	}
}

bool VulkanCore::CheckValidationLayersSupport()
{
	for (const char* layerName : mValidationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : mVKLayerProperties) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			throw std::runtime_error("Validation layers requested, but not available!");
			return false;
		}
	}
	return true;
}

bool VulkanCore::CheckExtensionsSupports()
{
	for (const char* layerName : mInstanceExtensions) {
		bool ExtensionFound = false;
		for (const auto& Extension : mVKInstanceExtensions) {
			if (strcmp(layerName, Extension.extensionName) == 0) {
				ExtensionFound = true;
				break;
			}
		}
		if (!ExtensionFound) {
			throw std::runtime_error("Validation layers requested, but not available!");
			return false;
		}
	}
	return true;
}
// End Create VK Instance

void VulkanCore::CreateSurface()
{
#if _WIN32
	VkWin32SurfaceCreateInfoKHR CreateInfo;
	ZeroVulkanStruct(CreateInfo, VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR);
	CreateInfo.hinstance = GetModuleHandle(nullptr);
	CreateInfo.pNext = nullptr;
	if (!mWindow)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		mWindow = glfwCreateWindow(1024, 1024, "Vulkan", nullptr, nullptr);
		CreateInfo.hwnd = glfwGetWin32Window(mWindow);
	}
	else
	{
		CreateInfo.hwnd = glfwGetWin32Window(mWindow);
	}
	if (VkResult Result = vkCreateWin32SurfaceKHR(mVkInstance, &CreateInfo, nullptr, &mVkSurface))
	{
		throw std::runtime_error("Faild to create window surface!");
	}
#endif // _WIN32

}

//Begin Selcted GPU And Create Deviece
void VulkanCore::SelecteGPUAndCreateDevice()
{
	//枚举所有的显卡，统计他们所支持的所有扩展
	unsigned int GPUCount;
	std::vector<VkPhysicalDevice> GPUs;
	vkEnumeratePhysicalDevices(mVkInstance, &GPUCount, nullptr);
	if (GPUCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
	else
	{
		GPUs.resize(GPUCount);
		mVkDeviceExtensions.resize(GPUCount);
		vkEnumeratePhysicalDevices(mVkInstance, &GPUCount, GPUs.data());
	}
	auto InitAvailableDevieceExtensions = [](VkPhysicalDevice GPU, TArray(VkExtensionProperties)& OutAvailableDeviceExtensions)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(GPU, nullptr, &extensionCount, nullptr);

		OutAvailableDeviceExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(GPU, nullptr, &extensionCount, OutAvailableDeviceExtensions.data());
	};

	for (int Index = 0; Index < GPUs.size(); Index++)
	{
		pair<int, std::vector<VkExtensionProperties>> GPUExtension;
		GPUExtension.first = Index;
		InitAvailableDevieceExtensions(GPUs[Index], mVkDeviceExtensions[Index]);
	}

	std::vector<int> integrateGpuIndexes;
	std::vector<int> discreteGpuIndexes;
	mVulkanDevieces.clear();
	//给所有的显卡创建VulkanDevice,搜集独显和集显
	int deviceIndex = -1;
	for (auto Gpu : GPUs)
	{
		deviceIndex ++;
		auto NewVulkanDevice = make_shared<VulkanDevice>(Gpu, mVkSurface);
		mVulkanDevieces.push_back(NewVulkanDevice);
		VulkanDevice::EGpuType gpuType = NewVulkanDevice->QueryGPUType();
		if (gpuType == VulkanDevice::EGpuType::Integrate)
		{
			integrateGpuIndexes.emplace_back(deviceIndex);
		}
		else if (gpuType == VulkanDevice::EGpuType::Discrete)
		{
			discreteGpuIndexes.emplace_back(deviceIndex);
		}
	}
	//挑选显卡
	if (discreteGpuIndexes.size() > 0)
	{
		for (auto Index : discreteGpuIndexes)
		{
			//挑选显卡时还需考虑是否Surface相关的属性
			//if ()
			//厂商/驱动版本等各种信息
			//mVulkanDevieces[Index]->GetGpuProperties().vendorID;
		}
		mEnabledDeviceIndex = discreteGpuIndexes[0];
		//选择一个独显
	}
	else if (integrateGpuIndexes.size() > 0)
	{
		for (auto Index : integrateGpuIndexes)
		{
			//厂商/驱动版本等各种信息
			//mVulkanDevieces[Index]->GetGpuProperties().vendorID;
			mEnabledDeviceIndex = Index;
		}
		//选择一个集显
	}
	else
	{
		//没有合适的显卡
	}
	if (mEnabledDeviceIndex != -1)
	{
		mVulkanDevieces[mEnabledDeviceIndex]->SetGpuFeature(mDeviceFeatures);
		mVulkanDevieces[mEnabledDeviceIndex]->SetGpuExtensions(mDeviceExtensions);
		mVulkanDevieces[mEnabledDeviceIndex]->SetVkDeviceExtensions(mVkDeviceExtensions[mEnabledDeviceIndex]);
		mVulkanDevieces[mEnabledDeviceIndex]->InitGPU();
	}
}
//End Selcted GPU And Create Deviece

void VulkanCore::CreateSwapChain()
{
	//这里东西后期应该调整到选择GPU的时候，作为考量GPU的标准
	VkPresentModeKHR desierdPresentMode =  VulkanSwapChain::GetDesierdPresentModel(mVulkanDevieces[mEnabledDeviceIndex], mVkSurface, bEnableVsync);

	VkSurfaceFormatKHR desierdSurfaceFormat = VulkanSwapChain::GetDesierdSurfaceFormat(mVulkanDevieces[mEnabledDeviceIndex], mVkSurface);
	VkSurfaceCapabilitiesKHR surfaceCapability = VulkanSwapChain::GetSurfaceCapabilities(mVulkanDevieces[mEnabledDeviceIndex], mVkSurface);

	mSwapChain = VulkanSwapChain::Create(mVulkanDevieces[mEnabledDeviceIndex], mVkSurface, surfaceCapability, desierdPresentMode, desierdSurfaceFormat);
	
}



//IMPL
shared_ptr<VulkanDevice> VulkanCore::GetDevice()
{
	if (mEnabledDeviceIndex != -1)
	{
		return mVulkanDevieces[mEnabledDeviceIndex];
	}
	return nullptr;
};