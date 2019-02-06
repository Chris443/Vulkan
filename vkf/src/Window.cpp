#include "../Window.h"

namespace vkf {

	Window::~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	Window::Window(uint32_t height, uint32_t width, std::string title)
	{
		this->height = height;
		this->width = width;
		if (!glfwInit()) {  
			return;
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	}

	void Window::createSurface(const VkInstance& instance) {
		VkResult success = VK_INCOMPLETE;
		success = glfwCreateWindowSurface(instance, window, NULL, &surface);
		assert(!success);
	}

	void Window::destroySurface(const VkInstance& instance) {
		vkDestroySurfaceKHR(instance, surface, NULL);
	}

	void Window::isResizable(bool resize) {
		glfwWindowHint(GLFW_RESIZABLE, resize);
	}

	void Window::recreate(uint32_t w, uint32_t h) {
		width = w;
		height = h;
		glfwSetWindowSize(window, width, height);
	}
}
