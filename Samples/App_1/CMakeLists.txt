
cmake_minimum_required(VERSION 2.8)
project (App_1)


SET (SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Application.h"
"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)


include_directories(${VKF_SOURCE_DIR}
                    ${App_1_SOURCE_DIR})

include_directories($ENV{VK_SDK_PATH}/include)
#VK SDK linking
if (WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            link_directories   ($ENV{VK_SDK_PATH}/Bin
                                $ENV{VK_SDK_PATH}/Lib
                                $ENV{VULKAN_SDK}/Bin
                                $ENV{VULKAN_SDK}/Lib)
    else()
            link_directories   ($ENV{VK_SDK_PATH}/Bin32
                                $ENV{VK_SDK_PATH}/Lib32
                                $ENV{VULKAN_SDK}/Bin32
                                $ENV{VULKAN_SDK}/Lib32)
    endif()
else()
    link_directories   ($ENV{VK_SDK_PATH}/x86_64/lib
                        $ENV{VULKAN_SDK}/lib
                        $ENV{VULKAN_SDK}/x86_64/lib)
endif()


include_directories(${CMAKE_SOURCE_DIR}/3rdparty/glm)
#glfw link include
link_directories   (${CMAKE_SOURCE_DIR}/3rdparty/glfw/lib-vc2015)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/glfw/include)

add_executable( App_1 ${SOURCES})

target_link_libraries( App_1 VKF)
target_link_libraries( App_1 glfw3)
target_link_libraries( App_1 vulkan-1)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res / DESTINATION "${CMAKE_BINARY_DIR}")




