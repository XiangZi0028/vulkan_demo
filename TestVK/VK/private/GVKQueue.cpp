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
void GVKQueue::GetGPUQueueFamilyProperties()
{
	uint32_t QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mDevice->GetCurrentGPU(), &QueueFamilyCount, nullptr);
	mQueueFamilyProperties.resize(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(mDevice->GetCurrentGPU(), &QueueFamilyCount, mQueueFamilyProperties.data());
}

void GVKQueue::InitQueueFamilyIndices()
{
	for (int Index = 0; Index < mQueueFamilyProperties.size(); Index++)
	{
		if (mQueueFamilyProperties[Index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			mQueueFamilyIndics.GraphicsFamily = Index;
		}
		else if (mQueueFamilyProperties[Index].queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			mQueueFamilyIndics.TransferFamily = Index;
		}
		else if (mQueueFamilyProperties[Index].queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			mQueueFamilyIndics.ComputeFamily = Index;
		}
	}
}
