#pragma once
#include <vulkan/vulkan.hpp>
#include <GVKDevice.h>
#include<optional>
#include <iostream>
using namespace std;
class GVKDevice;
struct QueueFamilyIndices {
	std::optional<uint32_t> TransferFamily;
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> ComputeFamily;
};

class GVKQueue : public shared_ptr<GVKQueue>
{
public:
	void InitGVKQueue();
	GVKQueue(GVKDevice* Device);
	~GVKQueue();

private:
	std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
	GVKDevice* mDevice;
	QueueFamilyIndices mQueueFamilyIndics;
private:
	void GetGPUQueueFamilyProperties();
	void InitQueueFamilyIndices();
};

