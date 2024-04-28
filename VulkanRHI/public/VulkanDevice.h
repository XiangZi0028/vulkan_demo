#pragma once
#include <vulkan/vulkan.h>
#include <optional>
#include "CommonMicro.h"
#include "VulaknCommonDefine.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> TransferFamily;
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> ComputeFamily;
	std::optional<uint32_t> PresentFamily;
};

class VulkanQueue;
struct VulkanQueues {
	std::optional<shared_ptr<VulkanQueue>> TransferQueue;
	std::optional<shared_ptr<VulkanQueue>> GraphicsQueue;
	std::optional<shared_ptr<VulkanQueue>> ComputeQueue;
	std::optional<shared_ptr<VulkanQueue>> PresentQueue;
};
class VulkanDevice : public enable_shared_from_this<VulkanDevice>
{
public:
	enum class EGpuType
	{
		Unknown,
		Other,
		Integrate,
		Discrete,
		Virtual,
		CPU
	};
public:
	VulkanDevice(VkPhysicalDevice InGpu, VkSurfaceKHR InSurfaceKHR)
		:mGpu(InGpu), mSurface(InSurfaceKHR), mGpuType(EGpuType::Unknown)
	{};
	
	~VulkanDevice() {};
	
	EGpuType QueryGPUType();

	void InitGPU();
	

private:
	void CreateDevice();
	VkSurfaceKHR mSurface;
	DefineMemberWithGetter(VkPhysicalDevice, Gpu)
	DefineMemberWithGetter(EGpuType, GpuType)
	DefineMemberWithGetter(VkDevice, Device)
	DefineMemberWithGetterSetter(VkPhysicalDeviceProperties, GpuProperties)

	DefineMemberWithGetterSetter(VkPhysicalDeviceFeatures, GpuFeature)
	DefineMemberWithGetterSetter(TArray(const char*), GpuExtensions)
	DefineMemberWithGetterSetter(TArray(VkExtensionProperties), VkDeviceExtensions)
	TArray(VkQueueFamilyProperties) mQueueFamliyProperties;
	DefineMemberWithGetter(QueueFamilyIndices, QueueFamilyIndices)
	DefineMemberWithGetter(VulkanQueues, Queues)
		
};
