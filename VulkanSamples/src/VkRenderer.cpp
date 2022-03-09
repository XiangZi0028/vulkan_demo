#include<VkRenderer.h>
#include <assert.h>

VulkanRender::~VulkanRender()
{
}

void VulkanRender::initialize()
{
	//����һ���հ״���
	createPresentationWindow(500, 500);

	//��ʼ��������
	//_swapChainObj->initializeSwapChain();

	//����ָ���أ���Ϊ������Ҫ�õ�ָ���
	createCmdPool();

	//��������������ɫͼ
	buildSwapChaintAndDepthImg();

}

bool VulkanRender::render()
{
	while (true)
	{
		MSG msg;   // message
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		RedrawWindow(_window, NULL, NULL, RDW_INTERNALPAINT);
	}
	return true;
}

void VulkanRender::createPresentationWindow(const int& windowWith, const int& windowHeight)
{
	_width = windowHeight;
	_height = windowHeight;
	//��ʼ��������ṹ��
	WNDCLASSEX winInfo;
	memset(&winInfo, 0, sizeof(WNDCLASSEX));
	
	winInfo.cbSize = sizeof(WNDCLASSEX);
	winInfo.lpfnWndProc = WndProc;
	winInfo.hInstance = _connection;
	winInfo.lpszClassName = _name;
	winInfo.cbClsExtra = 0;
	winInfo.cbWndExtra = 0;
	winInfo.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	winInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winInfo.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winInfo.lpszMenuName = nullptr;
	winInfo.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	//ע�ᴰ����
	if (!RegisterClassEx(&winInfo))
	{
		std::cout << "Create Window Failed!!-----RegisterClassEx wrong!";
		exit(1);
	}
	
	//ʹ��ע����ഴ������
	RECT wr = { 0, 0, _width, _height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	_window = CreateWindowEx(0, _name, _name,
							WS_OVERLAPPEDWINDOW |WS_VISIBLE |WS_SYSMENU,
							100, 100,
							wr.right-wr.left, wr.bottom-wr.top,
							nullptr, nullptr, _connection, nullptr);

	if(!_window)
	{
		std::cout << "Cannot create a window to draw" << std::endl;
		exit(1);
	}
	SetWindowLongPtr(_window, GWLP_USERDATA, (LONG_PTR)&_application);
}

void VulkanRender::setImageLayout(VkImage, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits srcAccessMask, const VkCommandBuffer& cmdBuf)
{
}

LRESULT VulkanRender::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	switch (uMsg)
	{
	case WM_CLOSE: PostQuitMessage(0); break;
	default: break;
	}
	//ʣ����¼�����ϵͳ��Ĭ�ϴ���������
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void VulkanRender::createCmdPool()
{
	VulkanDevice* deviceObj = _application->_deviceObj;
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.pNext = nullptr;
	cmdPoolInfo.flags = 0;
	cmdPoolInfo.queueFamilyIndex = deviceObj->_graphicsQueueFamilyIndex;
	
	VkResult res = vkCreateCommandPool(deviceObj->_device,&cmdPoolInfo,nullptr,&_cmdPool);
	assert(res == VK_SUCCESS);
}

void VulkanRender::createSwapChain()
{
}

void VulkanRender::createDepthImg()
{
}

void VulkanRender::buildSwapChaintAndDepthImg()
{
	//��ȡָ���ύ��Ӧ�Ķ���
	_device->getDeviceQueue();
	//��������������ȡ��ɫͼ����
	//_swapChainObj->createSwapChain();
	//�������ͼ
	createDepthImg();
}
