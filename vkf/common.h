#pragma once

/*********** COMPILER SPECIFIC PREPROCESSORS ***********/
#ifdef _WIN32
//#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#elif __linux__ 
#define VK_USE_PLATFORM_XCB_KHR
#else
#error "OS not supported!"
#endif

/***Standard Library***/
#include <vector>
#include <iostream>
#include <memory>
#include <mutex>
#include <assert.h>
#include <fstream>
#include <string>

/*******Vulkan*****/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include <vulkan/vulkan.hpp>

/*******glm*******/
#include "glm/glm.hpp"
