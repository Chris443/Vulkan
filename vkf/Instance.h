#pragma once
#include "common.h"

namespace vkf {
	/**
	* \brief Class to create and encapsulate the Vulkan Instance with corresponding layers and extensions
	*/
	class Instance
	{
	public:
		/**
		* \brief Function to create the Vulkan instance handle
		* \param layers The Instance Level Layers, which should be supported by the Instance
		* \param extensions The Instance Level Extensions, which should be supported by the Instance
		* \param applicationName Name of the application
		* \param engineName Name of the engine
		* \return A Vulkan Enum to describe the creation process of the Instance
		*/
		VkResult createInstance(std::vector<std::string>& layers, std::vector<std::string>& extensions, std::string applicationName, std::string engineName);
		/**
		* \brief Function to destroy the Vulkan Instance Handle
		*/
		void destroyInstance();
		/**
		* \return The Vulkan Instance Handle
		*/
		VkInstance getInstance() const { return instance; };
	private:
		/**
		* \brief Vulkan Instance Handle
		*/
		VkInstance instance;
	}; 
}