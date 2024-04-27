#include "VulkanRHI/public/VulkanRHI.h"
#include "VulkanRHI/public/VulkanDevice.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "VulkanRHI/public/VulkanSwapChain.h"
void VulkanCore::InitVKCore()
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
	instanceInfo.enabledExtensionCount = mInstanceExtensions.size();
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
	//Ŀǰ��֧�ֵ�ԭ������Ϊ���ƶ���VULKAN ��SDKλ�ã����������Լ���λ�õ���Ҫ����
	//VulkanSDK\1.3.231.1\Bin\vkconfig.exe
	//��֪��Դ�����Vulkan�Ļ�֧�ֲ�֧��
	mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
}

void VulkanCore::InitRequiredInstanceExtensions()
{
	unsigned int glfwRequiredExtensionCount = 0;
	const char** glfwRequiredInstanceExtension = glfwGetRequiredInstanceExtensions(&glfwRequiredExtensionCount);
	//VK_KHR_SWAPCHAIN_EXTENSION_NAME ���豸��չ
	for (int Index = 0; Index < glfwRequiredExtensionCount; Index++)
	{
		mInstanceExtensions.push_back(((*glfwRequiredInstanceExtension) + Index));
	}
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
	VkWin32SurfaceCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	CreateInfo.hinstance = GetModuleHandle(nullptr);
	if (mWindow)
	{
		CreateInfo.hwnd = glfwGetWin32Window(mWindow);
	}
	if (vkCreateWin32SurfaceKHR(mVkInstance, &CreateInfo, nullptr, &mVkSurface))
	{
		throw std::runtime_error("Faild to create window surface!");
	}
#endif // _WIN32

}

//Begin Selcted GPU And Create Deviece
void VulkanCore::SelecteGPUAndCreateDevice()
{
	//ö�����е��Կ���ͳ��������֧�ֵ�������չ
	unsigned int GPUCount;
	std::vector<VkPhysicalDevice> GPUs;
	vkEnumeratePhysicalDevices(mVkInstance, &GPUCount, nullptr);
	if (GPUCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
	else
	{
		//VulkanDeviece.resize(GPUCount);
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
	mVulkanDevieces.empty();
	//�����е��Կ�����VulkanDevice,�Ѽ����Ժͼ���
	int deviceIndex = -1;
	for (auto Gpu : GPUs)
	{
		deviceIndex ++;
		auto NewVulkanDevice = mVulkanDevieces.emplace_back(Gpu);
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
	//��ѡ�Կ�
	if (discreteGpuIndexes.size() > 0)
	{
		for (auto Index : discreteGpuIndexes)
		{
			//��ѡ�Կ�ʱ���迼���Ƿ�Surface��ص�����
			//if ()
			//����/�����汾�ȸ�����Ϣ
			//mVulkanDevieces[Index]->GetGpuProperties().vendorID;
		}
		mEnabledDeviceIndex = discreteGpuIndexes[0];
		//ѡ��һ������
	}
	else if (integrateGpuIndexes.size() > 0)
	{
		for (auto Index : integrateGpuIndexes)
		{
			//����/�����汾�ȸ�����Ϣ
			//mVulkanDevieces[Index]->GetGpuProperties().vendorID;
			mEnabledDeviceIndex = Index;
		}
		//ѡ��һ������
	}
	else
	{
		//û�к��ʵ��Կ�
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
	//���ﶫ������Ӧ�õ�����ѡ��GPU��ʱ����Ϊ����GPU�ı�׼
	unsigned int formatNum = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(mVulkanDevieces[mEnabledDeviceIndex]->GetGpu(), mVkSurface, &formatNum, nullptr);
	TArray(VkSurfaceFormatKHR) formats(formatNum);
	vkGetPhysicalDeviceSurfaceFormatsKHR(mVulkanDevieces[mEnabledDeviceIndex]->GetGpu(), mVkSurface, &formatNum, formats.data());

	unsigned int presentModesNum = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(mVulkanDevieces[mEnabledDeviceIndex]->GetGpu(), mVkSurface, &presentModesNum, nullptr);
	TArray(VkPresentModeKHR) presentModes(formatNum);
	vkGetPhysicalDeviceSurfacePresentModesKHR(mVulkanDevieces[mEnabledDeviceIndex]->GetGpu(), mVkSurface, &presentModesNum, presentModes.data());

	bool bSupportPresentModeMailbox = false;
	bool bSupportPresentModeFIFO = false;
	bool bSupportPresentModeImmediate = false;

	for (int Index = 0; Index < presentModesNum; ++Index)
	{
		switch (presentModes[Index])
		{
		case VK_PRESENT_MODE_MAILBOX_KHR:
			bSupportPresentModeMailbox = true;
			break;
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			bSupportPresentModeImmediate = true;
			break;
		case VK_PRESENT_MODE_FIFO_KHR:
			bSupportPresentModeFIFO = true;
			break;
		default:
			break;
		}
	}
	mEnbalePresentMode = VK_PRESENT_MODE_FIFO_KHR;
	
	if (!bEnableVsync && bSupportPresentModeImmediate)
	{
		mEnbalePresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	}
	else if (bSupportPresentModeMailbox)
	{
		mEnbalePresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	}
	else if (bSupportPresentModeFIFO)
	{
		mEnbalePresentMode = VK_PRESENT_MODE_FIFO_KHR;
	}
	else
	{
		mEnbalePresentMode = presentModes[0];
	}

	mSwapChain = (new VulkanSwapChain())->shared_from_this();
}