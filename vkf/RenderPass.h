#pragma once
#include "common.h"

namespace vkf {
	class Device;
	/**
	* \brief The RenderPass class encapsulates the RenderPass Handle and takes care of initialization and deleting. For initialization, use the Device.createRenderpass() function inside the Device class.
	*/
	class RenderPass
	{
	public:
		/**
		* \brief Creates a Renderpass with given Parameters. Use the device.createRenderpass() function instead of this method to create a Renderpass.
		* \param device The device which should create this Renderpass
		* \param attachmentDescriptions A vector of VkAttachmentDescription which describe the Attachments
		* \param subPassDescription A vector of VkSubpassDescription, which describe the Subpasses
		* \param subpassDependencies A vector of VkSubpassDependency which describe the dependencies between subpasses
		*/
		RenderPass(std::shared_ptr<Device> device, std::vector<VkAttachmentDescription> attachmentDescriptions,
			std::vector<VkSubpassDescription> subPassDescription, std::vector<VkSubpassDependency> subpassDependencies);
		~RenderPass();
		/**
		* \brief Getter Function to retrieve the Vulkan Handle of this Renderpass
		* \return The Vulkan Renderpass Handle of this class
		*/
		VkRenderPass getRenderpass() {	return renderpass; };
	private:
		std::shared_ptr<Device> device;
		VkRenderPass renderpass;
	};
}

