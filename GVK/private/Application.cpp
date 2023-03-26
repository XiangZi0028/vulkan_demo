#include"Application.h"
#include "GVKShaderModules.h"
#include "../../ProjectInfoPath.h"
#include "GVKInstance.h"
#include "GVKPipeline.h"
#include "GVKRenderPass.h"
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
    mPipeline = new GVKPipeline();
    mPipeline->CreatePipelineLayout();
    mVertexShader = new GVKShader(ShaderType::VertexShader,std::string(SHADER_PATH)+ "/Triangle_vert.spv");
    mFragmentShader = new GVKShader(ShaderType::FragmentShader,std::string(SHADER_PATH)+"/Triangle_frag.spv");
    mRenderPass = new GVKRenderPass();
    mRenderPass->CreateRenderPass();
    mRenderPass->SetPipeline(mPipeline);
    mRenderPass->SetShaders(mVertexShader,mFragmentShader, nullptr);
    mRenderPass->CreateGraphicsRenderPipeline();
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