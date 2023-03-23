#include "GVKQueue.h"

GVKQueue::GVKQueue(GVKDevice* Device)
	:mDevice(Device)
{
	GetGPUQueueFamilyProperties();
	InitQueueFamilyIndices();
}

GVKQueue::~GVKQueue()
{
}

void GVKQueue::InitGVKQueue()
{
	GetGPUQueueFamilyProperties();
	InitQueueFamilyIndices();
}

QueueFamilyIndices GVKQueue::GetQueueFamilyIndices()
{
	return this->mQueueFamilyIndics;
}

void GVKQueue::CreateDeviceQueue()
{
	vkGetDeviceQueue(mDevice->GetVKDevice(), mQueueFamilyIndics.GraphicsFamily.value(), 0, &mGraphicQueue);
	vkGetDeviceQueue(mDevice->GetVKDevice(), mQueueFamilyIndics.PresentFamily.value(), 0, &mPresentQueue);

	//vkGetPhysicalDeviceSurfaceSupportKHR(mDevice, i, surface, &presentSupport);
}

void GVKQueue::GetGPUQueueFamilyProperties()
{
	uint32_t QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mDevice->GetCurrentGPU(), &QueueFamilyCount, nullptr);
	mQueueFamilyProperties.resize(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(mDevice->GetCurrentGPU(), &QueueFamilyCount, mQueueFamilyProperties.data());
}

//这里这个Index可能有点问题，需要捋一下
void GVKQueue::InitQueueFamilyIndices()
{
	for (int Index = 0; Index < mQueueFamilyProperties.size(); Index++)
	{
		if (mQueueFamilyProperties[Index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			mQueueFamilyIndics.GraphicsFamily = Index;
		}
		/*if (mQueueFamilyProperties[Index].queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			mQueueFamilyIndics.TransferFamily = Index;
		}
		if (mQueueFamilyProperties[Index].queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			mQueueFamilyIndics.ComputeFamily = Index;
		}*/
		VkBool32 PresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(mDevice->GetCurrentGPU(), Index, mDevice->mInstance->mGVKSurfaceKHR->GetSUrface(), &PresentSupport);
		if(PresentSupport)
		{
			mQueueFamilyIndics.PresentFamily = Index;
		}
		if (mQueueFamilyIndics.PresentFamily.value() && mQueueFamilyIndics.GraphicsFamily.value())
		{
			break;
		}
	}
}
