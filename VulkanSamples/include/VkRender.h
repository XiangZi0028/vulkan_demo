#pragma once
class VulkanRender
{
public:
	VulkanRender(VulkanApplication *app,VulkanDevice *deviceObj);
	~VulkanRender();

	void initialize();
	bool render();

	//创建一个空的窗口 
	void createPresentationWindow(const int& windowWith=500,const int & windowHeight=500);
	void setImageLayout(VkImage, 
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkAccessFlagBits srcAccessMask,
		const VkCommandBuffer& cmdBuf);
	//设置窗口回调函数
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT UMsg, WPARAM wParam, LPARAM lParam);


private:

public: 
	HINSTANCE _connect;
	char name[80];//窗口名字
	int _width, _height;

};

