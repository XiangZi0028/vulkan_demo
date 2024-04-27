#pragma once
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

template<class T>
static void InitializeVkStructture(T& vkStruct, VkStructureType vkType)
{
	vkStruct.sType = vkType;
	memset(((uint8*)&vkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
}

#define NEW_SHARED(TypeConstruct)\
	(new TypeConstruct)->shared_from_this();