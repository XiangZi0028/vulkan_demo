#include "GVKInstance.h"

GVKInstance::GVKInstance(GLFWwindow* Window)
	:mWindow(Window)
{
	InitValidationLayers();
	EnumerateInstanceExtensionProperties();
	EnumerateInstanceLayerProperties();
	CheckValidationLayersSupport();
	CreateVKInstance();
	mGVKDevice = new GVKDevice(this);
	mGVKSurfaceKHR = new GVKSurfaceKHR();
	mGVKSurfaceKHR->CreatePlatformSurfaceKHR(mInstance, mWindow);
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
}


void GVKInstance::EnumerateInstanceExtensionProperties()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	mExtensions_VK.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, mExtensions_VK.data());

	std::cout << "available extensions:\n";
	for (const auto& extension : mExtensions_VK) {
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
	mGVKDevice->Cleanup();
	mGVKSurfaceKHR->Cleanup(mInstance);
	vkDestroyInstance(mInstance, nullptr);
}

void GVKInstance::InitValidationLayers()
{
	//mValidationLayers.insert(mValidationLayers.begin(),"VK_LAYER_KHRONOS_validation");
}