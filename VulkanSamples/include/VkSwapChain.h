#pragma once
#include <vulkan/vulkan.h>
#include<iostream>
#include<vector>
#include<VkApplication.h>
#include<VkRenderer.h>
/*
* ÿ����������ͼ��ͼ�����(image)��ʹ�ýӿ�(view)
*/
struct SwapChainBuffer
{
	VkImage image;
	VkImageView view;
};

//ǰ������

class VulkanRender;

class VulkanSwapChain
{
public:
	VulkanSwapChain(VulkanRender* renderer) :_rendererObj(renderer) {};
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
	//ϵͳƽ̨��ص��߼��������
	VkSurfaceKHR _surface;
	//�������õ��Ļ���ͼ������
	uint32_t _swapChainImgCnt;
	//����������
	VkSwapchainKHR _swapChain;
	//������ͼ����б�
	std::vector<SwapChainBuffer> _colorBufs;
	//��ǰ���õĻ��Ʊ�������
	uint32_t _currentColorBufIdx;
	//ͼ���ʽ
	VkFormat _format;

	PFN_vkQueuePresentKHR		fpQueuePresentKHR;
	PFN_vkAcquireNextImageKHR	fpAcquireNextImageKHR;

private:
	//����ͼ���������
	VkSurfaceCapabilitiesKHR _surfaceCapabilities;
	//����չʾģʽ������
	uint32_t _presentModeCnt;
	//չʾģʽ������
	std::vector<VkPresentModeKHR> _presentModes;
	//����������ɫͼ�ĳߴ�
	VkExtent2D _swapChainExtent;
	//������ɫͼ������
	uint32_t _desiredNumberOfSwapChainImgs;
	VkSurfaceTransformFlagBitsKHR _preTransform;
	//�����˽�����������Ӧ��չʾģʽ��ʶ ��λ��
	VkPresentModeKHR _swapChainPresentMode;
	//��ȡ���Ľ�������ɫ����
	std::vector<VkImage> _swapChainImgs;
	std::vector<VkSurfaceFormatKHR> _surfaceFormat;
	
	VulkanApplication* _appObj;
	VulkanRender* _rendererObj;


	PFN_vkGetPhysicalDeviceSurfaceSupportKHR		fpGetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		fpGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	fpGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkDestroySurfaceKHR							fpDestroySurfaceKHR;

	
	PFN_vkCreateSwapchainKHR	fpCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR	fpDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
};

