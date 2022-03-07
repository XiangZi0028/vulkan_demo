#include<VkRender.h>

VulkanRender::VulkanRender(VulkanApplication* app, VulkanDevice* deviceObj)
{
}

VulkanRender::~VulkanRender()
{
}

void VulkanRender::createPresentationWindow(const int& windowWith, const int& windowHeight)
{
	_width = windowHeight;
	_height = windowHeight;
	WNDCLASSEX winInfo;
	memset(&winInfo, 0, sizeof(WNDCLASSEX));
	winInfo.cbSize(sizeof(WNDCLASSEX));
	winInfo.lpfnWndProc(WndProc);
	winInfo.hInstance = _connect;
	winInfo.lpszClassName = name;

	//×¢²á´°¿ÚÀà
	

}

LRESULT VulkanRender::WndProc(HWND hWnd, UINT UMsg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
