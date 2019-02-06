#include "../Instance.h"

namespace vkf {
	VkResult Instance::createInstance(std::vector<std::string>& layers, std::vector<std::string>& extensions, std::string applicationName, std::string engineName) {
		//Vulkan Application Structure
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = NULL;
		appInfo.pApplicationName = applicationName.c_str();
		appInfo.applicationVersion = 1;
		appInfo.pEngineName = engineName.c_str();
		appInfo.engineVersion = 1;
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//Vulkan Instance Create Info Structure
		VkInstanceCreateInfo instInfo = {};
		instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instInfo.pNext = NULL;
		instInfo.flags = 0;
		instInfo.pApplicationInfo = &appInfo;

		std::vector<char const*> l;
		for (auto const& s : layers) {
			l.push_back(s.c_str());
		}

		//list of layers used in application
		instInfo.enabledLayerCount = (uint32_t)layers.size();
		instInfo.ppEnabledLayerNames = l.data();

		std::vector<char const*> e;
		for (auto const&s : extensions) {
			e.push_back(s.c_str());
		}

		//list of extensions used in application
		instInfo.enabledExtensionCount = (uint32_t)extensions.size();
		instInfo.ppEnabledExtensionNames = e.data();

		VkResult res = vkCreateInstance(&instInfo, NULL, &instance);
		return res;
	}

	void Instance::destroyInstance() {
		vkDestroyInstance(instance, NULL);
	}
}