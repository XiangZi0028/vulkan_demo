#include "GVKInstance.h"
#include "GVKSwapChain.h"
#include "VulkanGlobalInfo.h"
#include "GVKPipeline.h"
GVKInstance::GVKInstance(GLFWwindow* Window)
	:mWindow(Window)
{
	InitInstanceValidationLayers();
	InitRequiredInstanceExtensions();
	EnumerateInstanceExtensionProperties();
	EnumerateInstanceLayerProperties();
	CheckValidationLayersSupport();
	CheckExtensionsSupports();
	CreateVKInstance();
	mGVKSurfaceKHR = new GVKSurfaceKHR();
	mGVKSurfaceKHR->CreatePlatformSurfaceKHR(mInstance, mWindow);
	mGVKDevice = new GVKDevice(this);
    mSwapChain = new GVKSwapChain(mGVKDevice,mGVKSurfaceKHR,mWindow,mGVKDevice->GetQueue());
    GVKVariable::GGVKSwapChain = mSwapChain;
    mSwapChain->CreateVKSwapChain();
    mSwapChain->CreateImageViewsForSwapChainImages();
}

GVKInstance::~GVKInstance()
{

}


VkInstance GVKInstance::GetVKInstance()
{
	return mInstance;
}

bool GVKInstance::CheckValidationLayersSupport()
{
	for (const char* layerName : mValidationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : mAvailableLayers_VK) {
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

bool GVKInstance::CheckExtensionsSupports()
{
	for (const char* layerName : mInstanceExtensions) {
		bool ExtensionFound = false;
		for (const auto& Extension : mInstanceExtensions_VK) {
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

void GVKInstance::EnumerateInstanceExtensionProperties()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	mInstanceExtensions_VK.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, mInstanceExtensions_VK.data());

	std::cout << "available extensions:\n";
	for (const auto& extension : mInstanceExtensions_VK) {
		std::cout << '\t' << extension.extensionName << '\n';
	}
}

void GVKInstance::EnumerateInstanceLayerProperties()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount,nullptr);
	mAvailableLayers_VK.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, mAvailableLayers_VK.data());
	std::cout << "available Layers:\n";
	for (const auto& availableLayer : mAvailableLayers_VK) {
		std::cout << '\t' << availableLayer.layerName << '\n';
	}
}

void GVKInstance::CreateVKInstance()
{
	mExtensions_glfw = glfwGetRequiredInstanceExtensions(&mExtensionCount_glfw);

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = mAppNmae.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = mEngineName.c_str();
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = mExtensionCount_glfw;
	instanceInfo.ppEnabledExtensionNames = mExtensions_glfw;
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
	instanceInfo.ppEnabledLayerNames = mValidationLayers.size() == 0 ? nullptr : mValidationLayers.data();
	if (vkCreateInstance(&instanceInfo, nullptr, &mInstance) != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vkinstance!");
	}
}

void GVKInstance::Cleanup()
{
    mSwapChain->Cleanup();
	mGVKDevice->Cleanup();
	mGVKSurfaceKHR->Cleanup(mInstance);
	vkDestroyInstance(mInstance, nullptr);
}

void GVKInstance::InitInstanceValidationLayers()
{
	//目前不支持的原因是因为我移动了VULKAN 的SDK位置，如果想放在自己的位置的需要配置
	//VulkanSDK\1.3.231.1\Bin\vkconfig.exe
	//不知道源码编译Vulkan的话支持不支持
	mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
}

void GVKInstance::InitRequiredInstanceExtensions()
{
	//VK_KHR_SWAPCHAIN_EXTENSION_NAME 是设备扩展
	//mInstanceExtensions.push_back();
}