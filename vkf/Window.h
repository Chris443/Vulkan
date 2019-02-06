#pragma once
#include "common.h"

namespace vkf {
	/**
	* \brief Window class as interface for GLFW and Vulkan Surface. The surface is not initialized by default, so it has to be initialized by the createSurface() function manually.
	*/
	class Window
	{
	public:
		/**
		* Initializes GLFW and creates a GLFWWindow
		* \param height Height of the window
		* \param width Width of the window
		* \param title Bartitle of the window
		*/
		Window(uint32_t height, uint32_t width,std::string title);
		/**
		* Destroys the Window and terminates GLFW
		*/
		~Window();
		/**
		* \return The GLFW Window Handle
		*/
		GLFWwindow* getGLFWWindow() const { return window; }
		/**
		* \return The width of the window
		*/
		const uint32_t& getWidth() const{ return width; }
		/**
		* \return The height of the window
		*/
		const uint32_t& getHeight() const { return height; }
		/**
		* \return Returns the Height and Width of the Window as Vulkan Structure 
		*/
		VkExtent2D getExtents() const {	return VkExtent2D{ width,height };
		}
		/**
		* Creates the Vulkan Surface Handle
		* \param instance The instance to which the surface should be initialized
		*/
		void createSurface(const VkInstance& instance);
		/**
		* Destroys the Vulkan Surface Handle
		*/
		void destroySurface(const VkInstance& instance);
		/**
		* Sets the window resizability
		*/
		void isResizable(bool resize);
		/**
		* \return The Vulkan Surface Handle
		*/
		VkSurfaceKHR getSurface() const { return this->surface; }
		/**
		* Destroys the old Window and creates a new one with given Parameters. Use this function if you i.e. want to recreate the Swapchain.
		* \param width The width of the new Window
		* \param height The height of the new Window
		*/
		void recreate(uint32_t width, uint32_t height);
	private:
		/**GLFW Window Handle*/
		GLFWwindow* window;
		/**Window width*/
		uint32_t width;
		/**Window height*/
		uint32_t height;
		/**Vulkan surface Handle*/
		VkSurfaceKHR surface;
	};
}

