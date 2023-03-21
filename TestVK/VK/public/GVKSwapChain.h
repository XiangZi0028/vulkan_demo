#pragma once
#include <vulkan/vulkan.hpp>
#include <GVKDevice.h>
class GVKSwapChain : shared_ptr<GVKSwapChain>
{
public:
	GVKSwapChain(GVKDevice* Device, GVKSurfaceKHR Surface);
	~GVKSwapChain();
	
private:
	GVKDevice* mDevice;
	GVKSurfaceKHR* mSurface;

};
