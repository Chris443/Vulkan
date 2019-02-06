#include "../LayerAndExtensions.h"

namespace vkf {

	void LayerAndExtensions::initBasicLayersAndExtensions() {
		uint32_t glfwExtensionCount = 0;
		/* Enable surface extensions (platform-dependant) queried from GLWF */
		std::vector<std::string> ext = getRequiredGLFWExtensions();
		instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
		globalExtensionNames.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		globalExtensionNames.insert(globalExtensionNames.end(), ext.begin(), ext.end());
	}

	void LayerAndExtensions::initCustomLayersAndExtensions(std::vector<std::string> layerNames,	std::vector<std::string> globalExtensions)  {
		std::vector<VkLayerProperties> allSupportedLayers = getSupportedInstanceLayers();
		std::vector<VkExtensionProperties> allSupportedExtensions;
		bool isSupported = false;

		// check if all Layers are supported
		for (auto const& requestedLayer : layerNames) {
			for (auto const& supportedLayer : allSupportedLayers) {
				if ( strcmp(requestedLayer.c_str(), supportedLayer.layerName)  == 0) {
					isSupported = true;
					break;
				}
			}
			if (!isSupported) {
				std::cerr << "WARNING::LAYER_NOT_SUPPORTED: " << requestedLayer.c_str() << std::endl;
				std::cerr << std::endl;
			}
			isSupported = false;
		}

		//check if at least one Layer supports Extension(s)
		for (auto const& requestedExtensions : globalExtensions) {
			for (auto const& requestedLayer : layerNames) {
				std::vector<VkExtensionProperties> currentLayerExtensions = getSupportedInstanceExtensions(requestedLayer);
				for (auto const& supportedExtensions : currentLayerExtensions) {
					if (strcmp(supportedExtensions.extensionName, requestedExtensions.c_str()) == 0) {
						isSupported = true;
						break;
					}
				}
				if (isSupported) { break; }
			}
			if (!isSupported) {
				std::cerr << "WARNING::INSTANCE_EXTENSION_NOT_SUPPORTED: " << requestedExtensions.c_str() << std::endl;
				std::cerr << std::endl;
			}
			isSupported = false;
		}

		this->instanceLayers = layerNames;
		this->globalExtensionNames = globalExtensions;
		std::vector<std::string> glfwExtensions = getRequiredGLFWExtensions();
		globalExtensionNames.insert(globalExtensionNames.end(), glfwExtensions.begin(), glfwExtensions.end());
	}

	std::vector<VkLayerProperties> LayerAndExtensions::getSupportedInstanceLayers() const {
		uint32_t layerCount;
		std::vector <VkLayerProperties> globalLayers;
		vkEnumerateInstanceLayerProperties(&layerCount, NULL);
		globalLayers.resize(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, globalLayers.data());
		return globalLayers;
	}

	std::vector<VkExtensionProperties> LayerAndExtensions::getSupportedInstanceExtensions(const std::string layerName) const {
		uint32_t extensionCount;
		std::vector <VkExtensionProperties> globalExtensions;
		vkEnumerateInstanceExtensionProperties(layerName.c_str(), &extensionCount, NULL);
		globalExtensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(layerName.c_str(), &extensionCount, globalExtensions.data());
		return globalExtensions;
	}
	std::vector<VkExtensionProperties> LayerAndExtensions::getSupportedDeviceExtensionsByLayer(VkPhysicalDevice gpu, const std::string layerName) const{
		uint32_t extensionCount;
		std::vector<VkExtensionProperties> deviceExtensions;
		vkEnumerateDeviceExtensionProperties(gpu, layerName.c_str(), &extensionCount, NULL);
		deviceExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(gpu, layerName.c_str(), &extensionCount, deviceExtensions.data());
		return deviceExtensions;
	}

	std::vector<VkExtensionProperties> LayerAndExtensions::getSupportedDeviceExtensions(VkPhysicalDevice gpu) const
	{
		uint32_t extensionCount;
		std::vector<VkExtensionProperties> deviceExtensions;
		vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, NULL);
		deviceExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, deviceExtensions.data());
		return deviceExtensions;
	}

	std::vector<std::string> LayerAndExtensions::getRequiredGLFWExtensions() {
		uint32_t glfwExtensionCount = 0;
		/* Enable surface extensions (platform-dependant) queried from GLWF */
		std::vector<std::string> glfwExtensions;
		const char** extension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		for (unsigned int i = 0; i < glfwExtensionCount; i++)
		{
			std::string ext = *extension;
			glfwExtensions.push_back(ext);
			*(extension++);
		}
		return  glfwExtensions;
	}
}

