#include <iostream>
#include <VulkanRHI/public/VulkanRHI.h>
#include <CommonMicro.h>
using namespace std;

int main()
{
	
	shared_ptr<VulkanCore> VkCore(new VulkanCore());
	VkCore->InitVKCore();
	return 0;
}