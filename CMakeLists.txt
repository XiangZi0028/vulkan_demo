cmake_minimum_required(VERSION 3.10)


if (NOT T_LIBRARY_OUTPUT_PATH)
    set(T_LIBRARY_OUTPUT_PATH {CMAKE_BINARY_DIR}/Bin CACHE PATH INTERNAL)
endif ()

add_subdirectory(3rd)
#add_subdirectory(VulkanSamples)
add_subdirectory(TestVK)