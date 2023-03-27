#ifndef VLUKANDEMO_VULKANGLOBALINFO_H
#define VLUKANDEMO_VULKANGLOBALINFO_H

#include <vulkan/vulkan.hpp>
class GVKSwapChain;
class GVKDevice;
namespace GVKVariable
{
    extern VkDevice GDevice;
    extern GVKSwapChain* GGVKSwapChain;
    extern GVKDevice* GGVKDevice;
}


#endif