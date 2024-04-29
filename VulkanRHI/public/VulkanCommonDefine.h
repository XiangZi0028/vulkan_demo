#pragma once
#include<iostream>
using namespace std;
enum EVulkanCmdBufferPoolType
{
	Graphics,
	Compute,
	Transfer,
	Present
};

enum EVertexIAType
{
	IA_None = 0,
	IA_Position,
	IA_UV0,
	IA_UV1,
	IA_Normal,
	IA_Tangent,
	IA_Color,
	IA_SkinWeight,
	IA_SkinIndex,
	IA_SkinPack,
	IA_InstanceFloat1,
	IA_InstanceFloat2,
	IA_InstanceFloat3,
	IA_InstanceFloat4,
	IA_Custom0,
	IA_Custom1,
	IA_Custom2,
	IA_Custom3,
	IA_Count,
};

