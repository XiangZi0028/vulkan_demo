#include"Application.h"

Application::Application()
{

}

Application::~Application()
{
}
void Application::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Vulkan", nullptr, nullptr);
};
void Application::InitVulkan()
{
	mGVKInstance = new GVKInstance(mWindow);
};
void Application::MainLoop()
{
	while (!glfwWindowShouldClose(mWindow))
	{

		glfwPollEvents();
	}
}
void Application::Cleanup()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}
void Application::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}