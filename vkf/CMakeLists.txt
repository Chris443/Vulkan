cmake_minimum_required(VERSION 2.8)

PROJECT(VKF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
#VKSDK include
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

SET (SRC_LIST  "${CMAKE_CURRENT_SOURCE_DIR}/Buffer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/CommandBuffer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/CommandPool.h"
"${CMAKE_CURRENT_SOURCE_DIR}/common.h"
"${CMAKE_CURRENT_SOURCE_DIR}/DeskriptorPool.h"
"${CMAKE_CURRENT_SOURCE_DIR}/DeskriptorSet.h"
"${CMAKE_CURRENT_SOURCE_DIR}/DeskriptorSetLayout.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Device.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Fence.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Framebuffer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Image.h"
"${CMAKE_CURRENT_SOURCE_DIR}/ImageView.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Instance.h"
"${CMAKE_CURRENT_SOURCE_DIR}/LayerAndExtensions.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Mesh.h"
"${CMAKE_CURRENT_SOURCE_DIR}/PhysicalDevice.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Pipeline.h"
"${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayout.h"
"${CMAKE_CURRENT_SOURCE_DIR}/PipelineManager.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Queue.h"
"${CMAKE_CURRENT_SOURCE_DIR}/RenderPass.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Sampler.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Semaphore.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Shader.h"
"${CMAKE_CURRENT_SOURCE_DIR}/ShaderModule.h"
"${CMAKE_CURRENT_SOURCE_DIR}/ShaderRessourceManager.h"
"${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Swapchain.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Texture.h"
"${CMAKE_CURRENT_SOURCE_DIR}/tiny_obj_loader.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Util.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Vertex.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Window.h"

"${CMAKE_CURRENT_SOURCE_DIR}/src/Buffer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/CommandBuffer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/CommandPool.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/DeskriptorPool.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/DeskriptorSet.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/DeskriptorSetLayout.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Device.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Fence.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Framebuffer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/ImageView.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Instance.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/LayerAndExtensions.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/PhysicalDevice.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Pipeline.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineLayout.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Queue.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/RenderPass.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Sampler.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Semaphore.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/ShaderRessourceManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/ShaderModule.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Swapchain.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp"
)


include_directories(${CMAKE_SOURCE_DIR}/3rdparty/glm)
#glfw link include
link_directories   (${CMAKE_SOURCE_DIR}/3rdparty/glfw/lib-vc2015)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/glfw/include)

add_library(VKF STATIC ${SRC_LIST})

install(TARGETS VKF DESTINATION bin)

target_include_directories(VKF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})






