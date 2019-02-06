#pragma once
#include "common.h"
#include "RenderPass.h"
#include "ImageView.h"

//vector for imageview attachments (depthimage)

namespace vkf {
	class Device;
	class Framebuffer
		/**
		* \brief The Framebuffer class encapsulates the VkFramebuffer Handle and takes care of initialization and deleting. For initialization, use the Device.createFramebuffer() function inside the Device class.
		*/
	{
	public:
		/**
		* \brief Initializes the Vulkan Framebuffer with given Values.
		* \param device The Corresponding device to this Framebuffer
		* \param renderpass The Renderpass to this Framebuffer
		* \param attachments The attachments the Framebuffer should have( i.e. ImageView, DepthView )
		* \param extents Define the Dimensions of the Renderpass
		* \param layers Used for Multiview. Set to 1 for no MultiviewSupport.
		* \see RenderPass ImageView Device
		*/
		Framebuffer(std::shared_ptr<Device> device, std::shared_ptr<RenderPass> renderpass,
			std::vector<std::shared_ptr<ImageView>> attachments, VkExtent2D const& extents, uint32_t layers);
		~Framebuffer();
		/**
		* \brief Getter for the Vulkan Handle
		* \return The Vulkan Framebuffer Handle
		*/
		 VkFramebuffer getFramebuffer(){return framebuffer;}
	private:
		std::shared_ptr<Device> device;
		std::shared_ptr<RenderPass> renderpass;

		VkFramebuffer framebuffer;
	};

}