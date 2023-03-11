#pragma once
#include "GVKInstance.h"
#include "GVKDevice.h"
//https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families
int main()
{
	GVKInstance Instance;
	GVKDevice Device(Instance);
	//不知道为什么不能在析构函数里面去调用
	Instance.DestroyGVKInstance();
	return 0;
}