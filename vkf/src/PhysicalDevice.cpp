#include "../PhysicalDevice.h"

namespace vkf {

	VkFormatProperties PhysicalDevice::getFormatProperties(VkFormat format) {
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
		return formatProperties;
	}

	void PhysicalDevice::initPhysicalDevice(VkPhysicalDevice physicalDevice) {
		this->physicalDevice = physicalDevice;
		vkGetPhysicalDeviceFeatures(physicalDevice,&deviceFeatures);
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		queueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
	}

	void PhysicalDevice::initSurfaceData(std::shared_ptr<Window> window) {
		if (physicalDevice == VK_NULL_HANDLE) {
			std::cerr << "ERROR::PHYSICALDEVICE::SURFACE_INITIALIZATION::PHYSICALDEVICE_NOT_INITIALIZED" << std::endl;
			std::cerr << std::endl;
			return;
		}
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, window->getSurface(), &surfacecapabilities);

		uint32_t formatCount= 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, window->getSurface(), &formatCount, NULL);
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, window->getSurface(), &formatCount, formats.data());

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, window->getSurface(), &presentModeCount, NULL);
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, window->getSurface(), &presentModeCount, presentModes.data());
	}
	//returns an array with indices of queuefamilies which supporter graphics + presentation
	std::vector<uint32_t> PhysicalDevice::getGraphicsPresentQueueIndices(std::shared_ptr<Window> const& window) const {
		assert(queueFamilies.size() != 0);

		std::vector<uint32_t> indices;
		VkBool32 supportsPresentation = false;
		for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
			if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
				vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice, i, window->getSurface(), &supportsPresentation);
				if (supportsPresentation) {
					indices.push_back(i);
					supportsPresentation = false;
				}
			}
		}
		return indices;
	}

	std::vector<uint32_t> PhysicalDevice::getQueueFamilyIndices(VkQueueFlags flag) const {
		assert(queueFamilies.size() != 0);

		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
			if (queueFamilies[i].queueFlags & flag) {
				indices.push_back(i);
			}
		}
		return indices;
	}
}
