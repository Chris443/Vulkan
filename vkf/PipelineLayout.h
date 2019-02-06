#pragma once
#include "common.h"
#include "DeskriptorSetLayout.h"


namespace vkf {
class Device;
/**
* \brief The PipelineLayout class encapsulates the VkPipelineLayout Handle and takes care of initialization and deleting.
	For initialization, use the Device.createPipelineLayout() function inside the Device class.	
*/
	class PipelineLayout
	{
	public:
		/**
		* \brief Initializes the Vulkan PipelineLayout with given Parameters
		* \param device The device which creates the PipelineLayout
		* \param setLayouts An Array of DeskriptorSetLayouts
		* \param pushConstantRanges The push Constants which can be accessed by the Pipeline
		* \see Device DeskriptorSet 
		*/
		PipelineLayout(std::shared_ptr<Device> device, std::vector<std::shared_ptr<DeskriptorSetLayout>> setLayouts = {}, std::vector<VkPushConstantRange> pushConstantRanges = {});
		~PipelineLayout();
		/**
		* \brief Getter Function which returns the Vulkan Handle
		* \return The Vulkan PipelineLayout Handle
		*/
		VkPipelineLayout getPipelineLayout() { return pipelineLayout; };

	private:
		VkPipelineLayout pipelineLayout;
		std::shared_ptr<Device> device;
	};
}
