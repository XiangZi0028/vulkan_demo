#pragma once 
#include "CommonMicro.h"
#include "VulkanBufferResource.h"
#include "VulkanCommonDefine.h"
class VulkanCommandBuffer;
class VulkanDevice;
class VulkanBufferResource;

class VulkanIndexBuffer : public enable_shared_from_this<VulkanIndexBuffer>
{
public:
	~VulkanIndexBuffer() {};

	static shared_ptr<VulkanIndexBuffer> Create(shared_ptr<VulkanDevice>& inVulkanDevice, shared_ptr<VulkanCommandBuffer>& inCommandBuffer, const TArray(uint16_t)& inVertexData);
	static shared_ptr<VulkanIndexBuffer> Create(shared_ptr<VulkanDevice>& inVulkanDevice, shared_ptr<VulkanCommandBuffer>& inCommandBuffer, const TArray(uint32_t)& inVertexData);
private:
	template<typename DataType>
	void UploadBuffer(shared_ptr<VulkanCommandBuffer>& inCommandBuffer, TArray(DataType) inVertexData);


	VulkanIndexBuffer::VulkanIndexBuffer(shared_ptr<VulkanDevice> inDevice, uint32_t inIndexCount, VkIndexType inIndexType);
	
	 ;
	DefineMemberWithGetter(int, IndexCount)
	DefineMemberWithGetter(VkIndexType, VkIndexType)
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithGetterSetter(shared_ptr<VulkanBufferResource>, VulkanResourceBuffer);
};

class VulkanVertexBuffer : enable_shared_from_this<VulkanVertexBuffer>
{
public:
	~VulkanVertexBuffer()
	{

	};
	static shared_ptr<VulkanVertexBuffer> Create(shared_ptr<VulkanDevice> inDevice, shared_ptr<VulkanCommandBuffer> inCommandBuffer, const TArray(float)& inVertexData, const TArray(EVertexIAType)& inIATypes);
	static uint32_t GetVertexIATypesTotalSize(const TArray(EVertexIAType)& inIATypes);
	static uint32_t GetVertexIATypeSize(const EVertexIAType& inIAType);
	static VkFormat GetVertexIATypeFormat(const EVertexIAType& inType);
	uint32_t GetVertexCount() const { return mTotalDataSize / mSingleAttributeSize; };
private:

	VulkanVertexBuffer::VulkanVertexBuffer(shared_ptr<VulkanDevice> inDevice, TArray(EVertexIAType) inIATypes, const uint32_t& inSingleAttributeSize = 0, const uint32_t& totalDataSize = 0)
		: mDevice(inDevice)
		, mIATypes(inIATypes)
		, mSingleAttributeSize(inSingleAttributeSize)
		, mTotalDataSize(totalDataSize)
	{
	};
	void SetupVertexIADesc();
	void SetupVertexInputBindingDesc();
private:
	DefineMemberWithGetter(uint32_t,SingleAttributeSize);
	DefineMemberWithGetter(uint32_t,TotalDataSize);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithGetterSetter(TArray(EVertexIAType), IATypes);
	DefineMemberWithGetterSetter(shared_ptr<VulkanBufferResource>, VulkanResourceBuffer);
	DefineMemberWithGetter(TArray(VkVertexInputAttributeDescription), VertexInputAttributesDesc);
	DefineMemberWithGetter(VkVertexInputBindingDescription, VertexInputBindingDesc);
};


