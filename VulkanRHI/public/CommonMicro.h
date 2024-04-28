#pragma once
#include <vulkan/vulkan.h>
#include<iostream>
#include <vector>
using namespace std;
#define TArray(Type)\
	std::vector<Type>

#define DefineMemberWithGetter(TYPE, MEMBER_NAME)\
public:\
  TYPE Get##MEMBER_NAME() {\
    return m##MEMBER_NAME;\
  };\
private:\
  TYPE m##MEMBER_NAME;

#define DefineMemberWithGetterSetter(TYPE, MEMBER_NAME)\
public:\
  void Set##MEMBER_NAME(TYPE& in##MEMBER_NAME ) {\
    m##MEMBER_NAME = in##MEMBER_NAME;\
  }\
  DefineMemberWithGetter(TYPE, MEMBER_NAME)

#define DefineMemberWithRefGetter(TYPE, MEMBER_NAME)\
public:\
  TYPE##& Get##MEMBER_NAME##Ref() {\
    return m##MEMBER_NAME;\
  };\
private:\
  TYPE m##MEMBER_NAME;

#define DefineMemberWithGetterRefSetter(TYPE, MEMBER_NAME)\
public:\
  void Set##MEMBER_NAME(TYPE& in##MEMBER_NAME ) {\
    m##MEMBER_NAME = in##MEMBER_NAME;\
  }\
  DefineMemberWithRefGetter(TYPE, MEMBER_NAME)


//template<class T, class  ...Args>
//static shared_ptr<T> NewSharedObject(Args... args)
//{
//    return make_shared<T>(args...);
//}
//
//template<class T>
//static shared_ptr<T> NewSharedObject()
//{
//    return make_shared<T>();
//}


template<class T>
static void InitializeVkStructture(T& vkStruct, VkStructureType vkType)
{
	vkStruct.sType = vkType;
	memset(((uint8_t*)&vkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
}
