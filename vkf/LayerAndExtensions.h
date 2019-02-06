#pragma once
#include "common.h"



namespace vkf {
	/**
	* \brief Helper class to to get easy access to Instance/Global level Layers and Extensions
	*/
	class LayerAndExtensions
	{
	public:
		/**
		* \brief Function to initialize membervariables with minimum required Layers and Extensions
		*/
		void initBasicLayersAndExtensions();
		/**
		* \brief Function to initialize membervariables with provided layers and Extensions.
		* Layers and Extensions provided to the function are checked for support and printed to the console if no support is available.
		* GLFW Extensions are activated internally.
		* \param layerNames Requested Layers 
		* \param globalExtensions Requested Extensions
		*/
		void initCustomLayersAndExtensions(std::vector<std::string> layerNames,	std::vector<std::string> globalExtensions);
		/**
		* \brief Returns all supported Layers on the instance layer on your machine
		*/
		std::vector<VkLayerProperties> getSupportedInstanceLayers() const;
		/**
		* \brief Returns all supported Extensions on the instance layer by given Layer on your machine
		* \param layerName Layer, whose Extensions are requested
		*/
		std::vector<VkExtensionProperties> getSupportedInstanceExtensions(const std::string layerName) const;
		/**
		* \brief Returns supported Device Extensions for given Layer
		* \param layerName Layer, whose Extensions are requested
		* \param gpu graphics card, whose Extensions are requested
		* \return All supported Device Extension of given gpu and Layer
		*/
		std::vector<VkExtensionProperties> getSupportedDeviceExtensionsByLayer(VkPhysicalDevice gpu, const std::string layerName) const;
		/**
		* \brief Returns layer-independent supported Device Extensions 
		*/
		std::vector<VkExtensionProperties> getSupportedDeviceExtensions(VkPhysicalDevice gpu) const;
		/**
		* \return All instance Layer Names
	    */
		std::vector<std::string> getEnabledInstanceLayers() const { return this->instanceLayers; }
		/**
		* \return All Global Extension Names
		*/
		std::vector<std::string> getEnabledInstanceExtensions() const { return this->globalExtensionNames; }
	private:
		/**
		* \brief requested Instance Layers
		*/
		std::vector<std::string> instanceLayers;
		/**
		* \brief requested Instance Extensions
		*/
		std::vector<std::string> globalExtensionNames;
		/**
		* \brief Gets the Extensions required for GLFW
		*/
		std::vector<std::string> getRequiredGLFWExtensions();
	};

}
