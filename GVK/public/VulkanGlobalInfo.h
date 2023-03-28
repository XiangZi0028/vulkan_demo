#ifndef VLUKANDEMO_VULKANGLOBALINFO_H
#define VLUKANDEMO_VULKANGLOBALINFO_H

#include <vulkan/vulkan.hpp>
class GVKSwapChain;

namespace GVKVariable
{
    extern VkDevice GDevice;
    extern VkSurfaceKHR GSurface;
    extern GVKSwapChain* GGVKSwapChain;
}


#endif