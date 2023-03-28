#pragma once
#include <GLFW/glfw3.h>
#include<iostream>
#include <memory>
#include <memory>

using namespace std;
class GVKPipeline;
class GVKInstance;
class GVKShader;
class GVKRenderPass;
class Application : public enable_shared_from_this<Application>
{
public:
	Application();
	~Application();
	void Run();

    //
public:
    GVKShader *mVertexShader = nullptr;
    GVKShader *mFragmentShader = nullptr;
    GVKInstance* mGVKInstance = nullptr;
    GVKPipeline* mPipeline = nullptr;
    GVKRenderPass *mRenderPass= nullptr;

private:
	
	uint32_t mWindowHeight = 720;
	uint32_t mWindowWidth = 1080;
	GLFWwindow* mWindow;

private:	

	void InitWindow();
	
	void InitVulkan();

	void MainLoop();
	
	void Cleanup();
	
};

