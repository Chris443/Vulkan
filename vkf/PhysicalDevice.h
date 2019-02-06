#pragma once
#include "common.h"
#include "Window.h"

namespace vkf {
	/**
	* \brief The PhysicalDevice class initializes all important data of a PhysicalDevice. The selected Vulkan PhysicalDevice Handle has to be passed to this class, which takes
	* care internally of work like querying for Queuesupport, Data and Surface initialization. The Handle is also stored internally.
	* The PhysicalDevice is needed for logical Device creation and Surface initialization. 
	* \note The PhysicalDevice does not pick a specific device. Instead a VkPhysicalDevice picked before has to be passed.
	*/
	class PhysicalDevice
	{
	public:
		/**
		* \brief Function for initialization of the PhysicalDevice data. Always call this function first.
		* \param physicalDevice The Vulkan PhysicalDevice handle, which should initialize all important data
		*/
		void initPhysicalDevice(VkPhysicalDevice physicalDevice);
		/**
		* \brief Function for initialization of the Surface data. This function is called separately in case you dont want Surface support.
		* \param window The corresponding (GLFW) Window which surfacedata and compatibility should be queried.
		* \see Window
		*/
		void initSurfaceData(std::shared_ptr<Window> window);
		/**
		* \brief Getter for the PhysicalDevice Handle
		* \return The Vulkan PhysicalDevice Handle
		*/
		VkPhysicalDevice getPhysicalDevice() const { return this->physicalDevice; };
		/**
		* \brief Getter for the PhysicalDeviceFeatures
		* \return The Vulkan specific PhysicalDeviceFeatures  of this physicalDevice
		*/
		VkPhysicalDeviceFeatures getFeatures() const { return deviceFeatures; };
		/**
		* \brief Getter for the PhysicalDeviceProperties
		* \return The Vulkan PhysicalDeviceProperties of this physicalDevice
		*/
		VkPhysicalDeviceProperties getProperties() const { return deviceProperties; };
		/**
		* \brief Getter for the PhysicalDeviceMemoryProperties
		* \return The Vulkan PhysicalDeviceMemoryProperties of this physicalDevice
		*/
		VkPhysicalDeviceMemoryProperties getMemoryProperties() const { return memoryProperties; };
		/**
		* \brief Getter for the FormatProperties
		* \return The Vulkan FormatProperties of this physicalDevice
		*/
		VkFormatProperties getFormatProperties(VkFormat format);
		/**
		* \brief Getter for the SurfaceCapabilities. Not initialized if initSurface() wasn't called.
		* \return The Vulkan SurfaceCapabilities. 
		*/
		VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const {	return surfacecapabilities;	};
		/**
		* \brief Getter for all supported SurfaceFormats. Not initialized if initSurface() wasn't called.
		* \return A vector of the supported Vulkan SurfaceFormats by this physicalDevice and Surface.
		*/
		std::vector<VkSurfaceFormatKHR> getSurfaceFormats() const { return formats; };
		/**
		* \brief Getter for all supported PresentMode. Not initialized if initSurface() wasn't called.
		* \return A vector of the supported Vulkan Presentmodes by this physicalDevice and Surface.
		*/
		std::vector<VkPresentModeKHR> getPresentModes() const { return presentModes; };
		/**
		* \brief Getter for the Queuefamily and their Properties of this physicalDevice
		* \return A vector with all VkQueuefamilyProperties of this physicalDevice
		*/
		std::vector<VkQueueFamilyProperties> getQueueFamilies() const { return queueFamilies; };
		/**
		* \brief Function to return the indices of QueueFamilies with Graphics and Presentation support
		* \param window The window which should be queried for presentation support
		* \return Indices of Queuefamily with graphics and presentation support
		*/
		std::vector<uint32_t> getGraphicsPresentQueueIndices(std::shared_ptr<Window> const& window) const;
		/**
		* \brief Function to return the indices of QueueFamilies with specific Queueflag set(i.e. Graphics Bit)
		* \return Indices of Queuefamily with support of given flag set
		*/
		std::vector<uint32_t> getQueueFamilyIndices(VkQueueFlags flag) const;
	private:
		VkPhysicalDevice physicalDevice;

		VkPhysicalDeviceFeatures deviceFeatures;
		VkPhysicalDeviceProperties deviceProperties;
		std::vector<VkQueueFamilyProperties> queueFamilies;

		VkSurfaceCapabilitiesKHR surfacecapabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		VkPhysicalDeviceMemoryProperties memoryProperties;
		// Memory Properties
	};
}
