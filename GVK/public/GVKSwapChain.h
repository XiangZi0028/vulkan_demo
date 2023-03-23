#pragma once
#include <vulkan/vulkan.hpp>
#include <GVKDevice.h>
class GVKSwapChain : public enable_shared_from_this<GVKSwapChain>
{
public:
	GVKSwapChain(GVKDevice* Device, GVKSurfaceKHR Surface);
	~GVKSwapChain();
	
private:
	GVKDevice* mDevice;
	GVKSurfaceKHR* mSurface;

};
