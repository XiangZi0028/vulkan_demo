#pragma once
#include <VkApplication.h>
#include<VkSwapChain.h>
#include<VkPipeline.h>
#include<iostream>
#include<Windows.h>
//���ͼ�ζ�Ӧ�����ݽṹ
struct DepthAttachment
{
	//���ͼ�ĸ�ʽ
	VkFormat format;
	//���ͼ�����
	VkImage image;
	//���ͼ����ڴ�ռ� �� image ���а�
	VkDeviceMemory memory;
	//Ӧ�ó���ʹ��ͼ�����Ľӿڣ�Ӧ�ó�����ֱ��ʹ����ͼ�����Ҫͨ����ͼ����ķ�ʽ
	VkImageView view;
};


/*
* �������һ��Ψһ��չʾ���ڼ���ص�Ӧ�ó�����Դ
* �磺�豸������������ˮ�ߵ�
* Ӧ�ó�������ж����Ⱦ���ڣ�ÿ��Renderer����ά��һ��������չʾ�����Լ�������ص���Դ
*/
class VulkanSwapChain;
class VulkanRender
{
public:
	VulkanRender(VulkanApplication* app, VulkanDevice* deviceObj);


	~VulkanRender();
	/*
	* ��ʼ����Ⱦ��
	* 1.����һ������ά�ȵ�չʾ����	
	* 2.ִ�н���������Ҫ�ĸ���ǰ�ò���
	*	�磺��ѯ��������չ��API��������������ҵ�չʾ���Ӧ�Ķ��С���ȡ�����ڻ��Ƶ�ͼ���ʽ
	* 
	*/
	void initialize();
	
	bool render();

	/*
	* ����һ���հ׵Ĵ���
	* ����һ�������ĵ���ʽ����(���Ӵ���)
	* ������չ������ʽ
	*/
	void createPresentationWindow(const int& windowWith=500,const int & windowHeight=500);
	void setImageLayout(VkImage, 
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkAccessFlagBits srcAccessMask,
		const VkCommandBuffer& cmdBuf);
	//���ô��ڻص�����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT UMsg, WPARAM wParam, LPARAM lParam);


	void createCmdPool();
	void createSwapChain();
	void createDepthImg();
	void buildSwapChaintAndDepthImg();

	VulkanSwapChain& getSwapChain() { return *_swapChainObj; };
private:
	//��Ĺ����߳�Ա
	VulkanSwapChain* _swapChainObj;
	VulkanApplication* _application;
	//�����չʾ��������豸����
	VulkanDevice *_device;

public: 
	HINSTANCE _connection;
	char _name[80];//������ʾ��Ӧ�ó�������
	HWND _window;//���ڵľ��
	int _width, _height;//���ڵĿ�Ⱥ͸߶�
	//Depth img
	DepthAttachment _depthAttachment;
	VkCommandBuffer _cmdDepthImage;//���ͼ��ָ���
	VkCommandPool _cmdPool;//ָ���
	
};



