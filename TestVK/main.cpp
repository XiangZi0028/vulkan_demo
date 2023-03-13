#pragma once
#include <Application.h>
//https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families
int main()
{
	Application app;
	app.Run();
	//不知道为什么不能在析构函数里面去调用
	return 0;
}