#pragma once
#include <vulkan/vulkan.h>
#include<iostream>
#include<vector>
/*
* 每个交换链视图的图像对象(image)和使用接口(view)
*/
struct SwapChainBuffer
{
	VkImage image;
	VkImageView view;
};

class VulkanSwapChain
{
public:
	VulkanSwapChain();
	~VulkanSwapChain();

	void initializeSwapChain();
	void createSwapChain();
	void destroySwapChain();

private:
	VkResult createSwapChainExtensions();
	void getSupportedFormats();
	VkResult createSurface();
	uint32_t getGraphicsQueueWithPresentationSupport();
	void managePresentMode();
	void createSwapChainColorBufferImages();
	void createColorImageView(const VkCommandBuffer cmdbuf);

public:
	//系统平台相关的逻辑表面对象
	VkSurfaceKHR _surface;
	//交换链用到的缓存图像数量
	uint32_t _swapChainImgCnt;
	//交换链对象
	VkSwapchainKHR _swapChain;
	//交换链图像的列表
	std::vector<SwapChainBuffer> _colorBufs;
	//当前可用的绘制表面索引
	uint32_t _currentColorBufIdx;
	//图像格式
	VkFormat _format;
private:
	//保存图像表面属性
	VkSurfaceCapabilitiesKHR _surfaceCapabilities;
	//保存展示模式的数量
	uint32_t _presentModeCnt;
	//展示模式的数组
	std::vector<VkPresentModeKHR> _presentModes;
	//交换链中颜色图的尺寸
	VkExtent2D _swapChainExtent;
	//可用颜色图的数量
	uint32_t _desiredNumberOfSwapChainImgs;
	VkSurfaceTransformFlagBitsKHR _preTransform;
	//保存了交换链创建对应的展示模式标识 按位与
	VkPresentModeKHR _swapChainPresentMode;
	//获取到的交换链颜色对象
	std::vector<VkImage> _swapChainImgs;
	std::vector<VkSurfaceFormatKHR> _surfaceFormat;
};

