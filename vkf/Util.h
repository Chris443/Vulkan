#pragma once
#include "common.h"
#include "PhysicalDevice.h"

namespace vkf {

	bool isSupportedFormat(VkFormatProperties formatProperties, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	VkFormat findSupportedDepthFormat(PhysicalDevice physicalDevice, VkImageTiling tiling, VkFormatFeatureFlags featureflags);

	VkPresentModeKHR pickPresentationMode(PhysicalDevice physicalDevice);
}

