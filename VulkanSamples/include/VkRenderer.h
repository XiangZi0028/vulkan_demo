#pragma once
#include <VkApplication.h>
#include<VkSwapChain.h>
#include<VkPipeline.h>
#include<iostream>
#include<Windows.h>
//深度图形对应的数据结构
struct DepthAttachment
{
	//深度图的格式
	VkFormat format;
	//深度图像对象
	VkImage image;
	//深度图像的内存空间 和 image 进行绑定
	VkDeviceMemory memory;
	//应用程序使用图像对象的接口，应用程序不能直接使用视图对象而要通过视图对象的方式
	VkImageView view;
};


/*
* 负责管理一个唯一的展示窗口及相关的应用程序资源
* 如：设备、交换链、流水线等
* 应用程序可以有多个渲染窗口，每个Renderer负责维护一个独立的展示窗口以及所有相关的资源
*/
class VulkanRender
{
public:
	VulkanRender(std::shared_ptr<VulkanApplication> app, std::shared_ptr<VulkanDevice> deviceObj) :
		_application(app),
		_device(deviceObj) {};


	~VulkanRender();
	/*
	* 初始化渲染器
	* 1.创建一个给定维度的展示窗口	
	* 2.执行交换链所需要的各种前置操作
	*	如：查询交换链扩展的API、创建表面对象、找到展示层对应的队列、获取可用于绘制的图像格式
	* 
	*/
	void initialize();
	
	bool render();

	/*
	* 创建一个空白的窗口
	* 创建一个交叠的弹出式窗口(或子窗口)
	* 设置扩展窗口样式
	*/
	void createPresentationWindow(const int& windowWith=500,const int & windowHeight=500);
	void setImageLayout(VkImage, 
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkAccessFlagBits srcAccessMask,
		const VkCommandBuffer& cmdBuf);
	//设置窗口回调函数
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT UMsg, WPARAM wParam, LPARAM lParam);


	void createCmdPool();
	void createSwapChain();
	void createDepthImg();
	void buildSwapChaintAndDepthImg();
private:
	//类的管理者成员
	std::shared_ptr<VulkanSwapChain> _swapChainObj;
	std::shared_ptr<VulkanApplication> _application;
	//与这个展示层关联的设备对象
	std::shared_ptr<VulkanDevice> _device;

public: 
	HINSTANCE _connection;
	char _name[80];//窗口显示的应用程序名称
	HWND _window;//窗口的句柄
	int _width, _height;//窗口的宽度和高度
	//Depth img
	DepthAttachment _depthAttachment;
	VkCommandBuffer _cmdDepthImage;//深度图的指令缓存
	VkCommandPool _cmdPool;//指令池
	
};



