#ifndef VLUKANDEMO_GVKSWAPCHAIN_H
#define VLUKANDEMO_GVKSWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include <iostream>
#include "GLFW/glfw3.h"

using namespace std;
class GVKDevice;
class GVKSurfaceKHR;
class GVKQueue;
class GVKTexture;
class GVKSwapChain : public enable_shared_from_this<GVKSwapChain>
{
public:
	GVKSwapChain(GVKDevice* Device, GVKSurfaceKHR* Surface, GLFWwindow* Window,GVKQueue* Queue);
	~GVKSwapChain();
    void Cleanup();
	void CreateVKSwapChain();
    void GetSwapChainImage();
    void CreateImageViewsForSwapChainImages();
    VkExtent2D GetSwapChainExtent();
private:
    VkExtent2D mExtent2D{};
    std::vector<VkImage> mSwapChainImages;
    std::vector<VkImageView> mSwapChainImageViews;
    uint32_t SwapChainImageCount = 0;
    GVKQueue *mQueue;
	GVKDevice* mDevice;
	GVKSurfaceKHR* mSurface;
    GLFWwindow *mWindow;
    VkSwapchainKHR mSwapChain;
};
#endif //VLUKANDEMO_GVKSWAPCHAIN_H