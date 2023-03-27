#ifndef VLUKANDEMO_GVKSWAPCHAIN_H
#define VLUKANDEMO_GVKSWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include <iostream>
#include "GLFW/glfw3.h"
#include <memory>
using namespace std;
class GVKDevice;
class GVKSurfaceKHR;
class GVKQueue;
class GVKTexture;
class GVKFrameBuffer;
class GVKRenderPass;
class GVKSwapChain : public enable_shared_from_this<GVKSwapChain>
{
public:
	GVKSwapChain(GVKDevice* Device, GVKSurfaceKHR* Surface, GLFWwindow* Window,GVKQueue* Queue);
	~GVKSwapChain();
    void Cleanup();
	void CreateVKSwapChain();
    void GetSwapChainImage();
    void CreateImageViewsForSwapChainImages();
    void SetRenderPass(GVKRenderPass* RenderPass);
    VkExtent2D GetSwapChainExtent();
    VkFormat GetSwapChainImgFormat();
private:
    VkExtent2D mExtent2D{};
    std::vector<VkImage> mSwapChainImages;
    std::vector<VkImageView> mSwapChainImageViews;
    std::vector<GVKFrameBuffer*> mFrameBuffer;
    std::vector<GVKTexture*> mFrameBufferTexture;
    std::vector<std::vector<GVKTexture*>> mFrameBufferAttachments;
    uint32_t SwapChainImageCount = 0;
    GVKQueue *mQueue;
	GVKDevice* mDevice;
	GVKSurfaceKHR* mSurface;
    GLFWwindow *mWindow;
    VkSwapchainKHR mSwapChain;
};
#endif //VLUKANDEMO_GVKSWAPCHAIN_H