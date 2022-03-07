#pragma once
class VulkanRender
{
public:
	VulkanRender(VulkanApplication *app,VulkanDevice *deviceObj);
	~VulkanRender();

	void initialize();
	bool render();

	//����һ���յĴ��� 
	void createPresentationWindow(const int& windowWith=500,const int & windowHeight=500);
	void setImageLayout(VkImage, 
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkAccessFlagBits srcAccessMask,
		const VkCommandBuffer& cmdBuf);
	//���ô��ڻص�����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT UMsg, WPARAM wParam, LPARAM lParam);


private:

public: 
	HINSTANCE _connect;
	char name[80];//��������
	int _width, _height;

};

