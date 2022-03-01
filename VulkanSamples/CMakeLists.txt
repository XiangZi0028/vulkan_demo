cmake_minimum_required(VERSION 3.10)
project(VULKAN_T)
set(Recipe_Name "3_0_DeviceHandshake")


#AUTO_LOCATE_VULKAN ON/OFF
#ON 使用cmake自动查找vulkan sdk的路径
#OFF 手动设置Vulkan SDK的路径
option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" OFF)

if(FALSE)
	message(STATUS "attempting auto locate vulkan using cmake ...")
	#通过cmake的vulkan模块查找vulkan路径
	#这里会自动返回一个布尔变量vulkan_found 它表示vulkan是否已经被自动找到
	#头文件路径 vulkan_include_dirs
	#必备库文件 vulkan_library
	find_package(Vulkan)
	if(${VULKAN_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${VULKAN_INCLUDE_DIRS})
		STRING(REGEX REPALCE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
	endif()
	if(NOT Vulkan_FOUND)
	#cmake可能无法自动定位库文件的位置 但是我们可以提供一些vulkansdk的文件位置
	#来确定vulkan_include_dirs 进而从中抽取正确的路径
	message(STATUS "Failed to locate vulkan sdk, retrying again ..")
		if(EXISTS "${VULKAN_PATH}")
			message(STATUS "successfully located the vulkan sdk: ${VULKAN_PATH}")
		else()
			message("ERROR:Unable to locate vulkan sdk.please turn off auto locate option by specifying 'AUTO_LOCATTE_PATH' as OFF")
			message("and specify manually path using vulkan_sdk and vulkan_version variables in the cmakelists.txt")
				return()
		endif()
	endif()
else()
	set(VULKAN_SDK "C:/VulkanSDK")
	set(VULKAN_VERSION "1.3.204.0")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	#我这里内部依赖了

	set(VULKAN_PATH ${PROJECT_SOURCE_DIR}/../3rd/vulkan)
	message(STATUS "Using manual specified path :${VULKAN_PATH}")
	if(NOT EXISTS "${VULKAN_PATH}")
		message("手动设置vulkan sdk 路径失败 请在cmake中设置VULKAN_SDK 和 VULKAN_VERSION")
		return ()
	endif()
endif()

include_directories(${VULKAN_PATH}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(src SRC_FILES)
add_executable(vktest main.cpp ${SRC_FILES})
target_link_libraries(vktest ${VULKAN_PATH}/lib/vulkan-1.lib)