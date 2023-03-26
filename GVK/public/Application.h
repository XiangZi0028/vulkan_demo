#pragma once
#include <GLFW/glfw3.h>
#include<iostream>
#include <memory>
#include "GVKInstance.h"
#include <memory>
using namespace std;
class GVKInstance;
class Application : public enable_shared_from_this<Application>
{
public:
	Application();
	~Application();
	void Run();
private:
	
	uint32_t mWindowHeight = 720;
	uint32_t mWindowWidth = 1080;
	GLFWwindow* mWindow;
	GVKInstance* mGVKInstance = nullptr;

private:	

	void InitWindow();
	
	void InitVulkan();

	void MainLoop();
	
	void Cleanup();
	
};

