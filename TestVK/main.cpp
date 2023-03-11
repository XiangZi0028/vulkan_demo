/*
 * Vulkan Program
 *
 * Copyright (C) 2016 Valve Corporation
 * Copyright (C) 2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
 Vulkan C++ Project Template
 Create and destroy a simple Vulkan instance.
 */

#include<vulkan/vulkan.hpp>
#include <iostream>
#include <vector>

struct VKData
{
    VkInstance instance;
    VkPhysicalDeviceProperties physicalDeviceProp;
};

int main()
{
    // Use validation layers if this is a debug build
    std::vector<const char*> layers;
#if defined(_DEBUG)
    layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
    VKData vkData;
 
    VkApplicationInfo applicationInfo;
    applicationInfo.apiVersion = VK_API_VERSION_1_2;
    applicationInfo.applicationVersion = 0;
    applicationInfo.pApplicationName = "TestVulkan";
    applicationInfo.pNext = nullptr;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    //applicationInfo.engineVersion = 1;
    applicationInfo.pEngineName = "Name";

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.enabledExtensionCount = 0;
    instanceCreateInfo.enabledLayerCount = 1;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.ppEnabledLayerNames = layers.data();
    instanceCreateInfo.ppEnabledExtensionNames = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

   
   VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &vkData.instance);

    if (result != VkResult::VK_SUCCESS)
    {
        std::cout << "Failed to create instance!!" << std::endl;
        return 1;
    }

    uint32_t gpuCount;
    std::vector<VkPhysicalDevice> gpuList;
    vkEnumeratePhysicalDevices(vkData.instance, &gpuCount,nullptr);
    if (gpuCount > 0)
    {
        vkEnumeratePhysicalDevices(vkData.instance, &gpuCount, gpuList.data());
    }
    gpuList.resize(gpuCount);
    if (gpuList.size() <= 0)
    {
        std::cout << "None GPU!" << std::endl;
        return 1;
    }
    //
     
    vkGetPhysicalDeviceProperties(gpuList[0], &(vkData.physicalDeviceProp));





    return 0;
}