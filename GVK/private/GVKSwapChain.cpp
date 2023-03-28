//
// Created by Administrator on 2023/3/23.
//
#include <GVKSwapChain.h>
#include <GVKDevice.h>
#include "GVKFrameBuffer.h"
#include "GVKRenderPass.h"
#include <GVKTexture.h>
GVKSwapChain::GVKSwapChain(GVKDevice* Device, GVKSurfaceKHR* Surface, GLFWwindow* Window, GVKQueue *Queue)
:mDevice(Device),mSurface(Surface),mWindow(Window),mQueue(Queue)
{

}
GVKSwapChain::~GVKSwapChain()
{

}

void GVKSwapChain::CreateVKSwapChain()
{
    VkSurfaceFormatKHR SurfaceFormat =  mSurface->ChooseSurfaceFormat(mDevice->GetCurrentGPU());
    VkPresentModeKHR PresentMode = mSurface->ChoosePresentMode(mDevice->GetCurrentGPU());
    mExtent2D = mSurface->ChooseExtent2D(mDevice->GetCurrentGPU(),mWindow);
    auto SurfaceCapabilities = mSurface->GetSurfaceCapabilities(mDevice->GetCurrentGPU());
    SwapChainImageCount = SurfaceCapabilities.minImageCount + 1;
    if(SurfaceCapabilities.maxImageCount > 0 && SwapChainImageCount > SurfaceCapabilities.maxImageCount)
    {
        SwapChainImageCount = SurfaceCapabilities.maxImageCount;
    }
    VkSwapchainCreateInfoKHR CreateInfo;
    CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    CreateInfo.surface = mSurface->GetSUrface();
    CreateInfo.minImageCount = SwapChainImageCount;
    CreateInfo.imageExtent = mExtent2D;
    CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
    CreateInfo.imageFormat= SurfaceFormat.format;
    CreateInfo.presentMode = PresentMode;
    CreateInfo.imageArrayLayers = 1;
    CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if(mQueue->GetGraphicQueue() != mQueue->GetPresentQueue())
    {
        uint32_t QueueFamilyIndices[] = {mQueue->GetQueueFamilyIndices().GraphicsFamily.value(), mQueue->GetQueueFamilyIndices().PresentFamily.value()};
        CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        CreateInfo.queueFamilyIndexCount = 2;
        CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
    }
    else
    {
        CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        CreateInfo.queueFamilyIndexCount = 0;
        CreateInfo.pQueueFamilyIndices = nullptr;
    }
    CreateInfo.preTransform = SurfaceCapabilities.currentTransform;
    CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    CreateInfo.clipped = VK_TRUE;
    //֮ǰ��Ϊû���������������� ����createswapchain��ʱ���������
    CreateInfo.oldSwapchain = VK_NULL_HANDLE;
    CreateInfo.pNext = nullptr;
    CreateInfo.flags = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR;
    //CreateInfo.flags =  VK_SWAPCHAIN_CREATE_FLAG_BITS_MAX_ENUM_KHR; //?? ���ﲻ����flagsû��ʲô����  ������֤��򿪵�ʱ����д�����ʾ��Ϣ
    if(vkCreateSwapchainKHR(mDevice->GetVKDevice(), &CreateInfo, nullptr, &mSwapChain))
    {
        throw std::runtime_error("Faild to Create SwapChain!");
    }
}

void GVKSwapChain::Cleanup()
{
    for(auto FrameBuffer : mFrameBuffer)
    {
        vkDestroyFramebuffer(mDevice->GetVKDevice(),FrameBuffer->GetVKFrameBuffer(), nullptr);
    }
    for(auto ImageView : mSwapChainImageViews)
    {
        vkDestroyImageView(mDevice->GetVKDevice(), ImageView, nullptr);
    }
    vkDestroySwapchainKHR(mDevice->GetVKDevice(), mSwapChain, nullptr);
}

void GVKSwapChain::GetSwapChainImage()
{
    uint32_t ImageCount;
    vkGetSwapchainImagesKHR(mDevice->GetVKDevice(), mSwapChain, &SwapChainImageCount, nullptr);
    mSwapChainImages.resize(SwapChainImageCount);
    vkGetSwapchainImagesKHR(mDevice->GetVKDevice(), mSwapChain, &SwapChainImageCount, mSwapChainImages.data());
}

void GVKSwapChain::CreateImageViewsForSwapChainImages()
{
    GetSwapChainImage();
    mSwapChainImageViews.resize(SwapChainImageCount);
    mFrameBufferAttachments.resize(SwapChainImageCount);
    for(int Index =0; Index < SwapChainImageCount; Index++ )
    {
        VkImageViewCreateInfo CreateInfo = {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        CreateInfo.image = mSwapChainImages[Index];
        CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        CreateInfo.format = mSurface->ChooseSurfaceFormat(mDevice->GetCurrentGPU()).format;
        CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        CreateInfo.subresourceRange.baseMipLevel = 0;
        CreateInfo.subresourceRange.levelCount = 1;
        CreateInfo.subresourceRange.baseArrayLayer = 0;
        CreateInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(mDevice->GetVKDevice(), &CreateInfo, nullptr, &mSwapChainImageViews[Index]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
        mFrameBuffer.emplace_back(new GVKFrameBuffer(mExtent2D.width, mExtent2D.height));
        mFrameBufferTexture.emplace_back(new GVKTexture(mExtent2D.width, mExtent2D.height));
        mFrameBufferTexture[Index]->SetVKImageAndView(mSwapChainImages[Index],mSwapChainImageViews[Index]);
        mFrameBufferAttachments[Index].push_back(mFrameBufferTexture[Index]);
        mFrameBuffer[Index]->SetAttachments(mFrameBufferAttachments[Index]);
    }
}

VkExtent2D GVKSwapChain::GetSwapChainExtent()
{
    return mExtent2D;
}

VkFormat GVKSwapChain::GetSwapChainImgFormat()
{
    return mSurface->ChooseSurfaceFormat(mDevice->GetCurrentGPU()).format;
}


void  GVKSwapChain::SetRenderPass(GVKRenderPass *RenderPass)
{
    for(int Index =0; Index < SwapChainImageCount; Index++ )
    {
        RenderPass->SetFrameBuffer(mFrameBuffer[Index]);
        mFrameBuffer[Index]->CreateFrameBuffer();
    }
}