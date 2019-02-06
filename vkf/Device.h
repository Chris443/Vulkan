#pragma once
#include "common.h"
#include "LayerAndExtensions.h"
#include "PhysicalDevice.h"
#include "Swapchain.h"
#include "Image.h"

#include "ShaderModule.h"
#include "Pipeline.h"
#include "PipelineLayout.h"

#include "Framebuffer.h"
#include "RenderPass.h"
#include "CommandPool.h"
#include "Queue.h"
#include "Buffer.h"
#include "DeskriptorSetLayout.h"
#include "DeskriptorPool.h"
#include "DeskriptorSet.h"
#include "Semaphore.h"
#include "Sampler.h"
#include "Texture.h"

namespace vkf {
	/**
	* \brief Simple structure to wrap the informations for the VkDeviceQueueCreateInfo's inside the Device creation.
	* Used for Queue creation.
	*/
	struct DeviceQueueCreateInfo
	{
		/**
		* \param queueFamilyIndex The index of the QueueFamily which should be created
		* \param queuePriorities A Vector of normalized floats for Queue priorities.
		* \note
		*	The amount of queuePriorities equals the amount of created Queues!
		*	i.e. if you want to create 3 Queues of queueFamily 0 you'd have to assign them like this: <br>
		*   DeviceQueueCreateInfo info(0, {0.2,0.5,1.0});
		*   Don't forget that you still have to retrieve the Queue Handle by calling createQueue();
		*/
		DeviceQueueCreateInfo(uint32_t queueFamilyIndex, std::vector<float> queuePriorities);
		/**
		* \brief The Index to the QueueFamily of which the Queues should be created.
		*/
		uint32_t            queueFamilyIndex;
		/**
		* \brief This vector contains the QueuePriorities, but the size of the vector also defines how many Queues should be created,
		* corresponding to the QueueFamily assigned in the Membervariable queueFamilyIndex.
		*/
		std::vector<float>  queuePriorities;
	};

	/**
	* \brief The core class of the Framework. This class should be used to create any Vulkan Object. Every Object encapsulates the Vulkan Handles. The Constructors and Destructors
	* take care of initialization and deletion of the Handles. Therefore every create() Function returns a shared_ptr. That way, the handles which pointers to are lost but
	* are still needed by other classes are still stored inside other classes. For creating the Device, use the create() function and not the Constructor, who is used
	* internally.
	*/
	class Device : public std::enable_shared_from_this<Device> //passing a shared_ptr of 'this'
	{
	public:
		/**
		* \brief Constructor for internal use. Don't create a Device this way.Creation this way leads to unknown behavior. Use the create() function instead.
		*/
		Device(VkInstance const& instance, std::vector<DeviceQueueCreateInfo> deviceQueueCreateInfos, PhysicalDevice physicalDevice,
			std::vector<std::string> deviceExtensionNames, std::vector<std::string> layerNames, VkPhysicalDeviceFeatures const& enabledFeatures);

		/**
		* \brief Function to create a logical Device. It is possible to create multiple logical Devices from one physical Device, but this is very unsafe and should be avoided.
		* \param instance The corresponding Instance this Device belongs to
		* \param deviceQueueCreateInfos A vector which holds the Information for QueueCreation. See DeviceQueueCreateInfo
		* \param physicalDevice The PhysicalDevice from which the logical Device should be created.
		* \param deviceExtensionNames The device-dependent Extensions which should be activated.
		* \param layerNames The Layers which should be activated.
		* \param enabledFeatures The Features which should be enabled for this logical Device i.e. TesselationShader
		* \return A Shared Pointer to a logical Device
		* \see Instance DeviceQueueCreateInfo PhysicalDevice LayerAndExtensions
		*/
		static std::shared_ptr<Device> create(VkInstance const& instance, std::vector<DeviceQueueCreateInfo> deviceQueueCreateInfos,
			PhysicalDevice physicalDevice, std::vector<std::string> deviceExtensionNames, std::vector<std::string> layerNames, VkPhysicalDeviceFeatures const& enabledFeatures);
		/**
		* \brief Creates a Swapchain by this Device
		* \param surface The Surface which should be used by the Swapchain for presentation
		* \param minImageCount The minimum amount of Images which should be used. i.e. 2 for DoubleBuffering
		* \param imageFormat The format which should be used
		* \param colorSpace The colorSpace which should be used to interpret the images
		* \param imageExtent The size in each Dimension of the Swapchain in pixels
		* \param imageArrayLayers The image Array layers. Used for stereoscopic drawing, i.e. 3D images for VR
		* \param imageUsage Describes the usage of the Swapchain
		* \param imageSharingMode The sharing mode used for the swapchain
		* \param queueFamilyIndices Defines which QueueFamily have access to the swapchain
		* \param preTransform Describes the Transformation of the Images before the Presentation
		* \param compositeAlpha Describes the Alpha compositing mode to use
		* \param presentMode The presentation Mode which should be used
		* \param clipped Describes if Pixels outside the Presentation region should be discarded
		* \param oldSwapchain The old Swapchain. Used for Swapchainrecreation.
		* \return A shared pointer to a Swapchain
		* \see Window 
		*/
		std::shared_ptr<Swapchain> createSwapchain(VkSurfaceKHR const& surface, uint32_t minImageCount, VkFormat imageFormat, VkColorSpaceKHR colorSpace, VkExtent2D imageExtent,
			uint32_t imageArrayLayers, VkImageUsageFlags imageUsage, VkSharingMode imageSharingMode, std::vector<uint32_t> queueFamilyIndices,
			VkSurfaceTransformFlagBitsKHR preTransform, VkCompositeAlphaFlagBitsKHR compositeAlpha, VkPresentModeKHR presentMode, VkBool32 clipped, std::shared_ptr<Swapchain> oldSwapchain);

		//std::shared_ptr<Image> createImage(VkImage image);
		/**
		* \brief Creates and initializes a Vulkan Image Handle.
		* \param imageType The Type if the Image, i.e. VK_IMAGE_TYPE_1D, VK_IMAGE_TYPE_2D
		* \param format The image Format
		* \param extent The extent on each image dimension
		* \param mipLevels Describes the number of levels for mipmapping
		* \param arrayLayers number of image layers
		* \param samples Used for multisampling. Describes the number of Subdata element samples
		* \param tiling Specifies the tiling arrangement in memory
		* \param usage A bit that Describes the usages of the Image, i.e. VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
		* \param sharingMode Describes the Acces of the image for Queue Families
		* \param queueFamilyIndices The list of QueueFamily which will have access to the Image
		* \param initialLayout THe initial Layout of all image subresources
		* \param physicalDevice The physicalDevice, which is used for memory Allocation
		*/
		std::shared_ptr<Image> createImage(VkImageType imageType, VkFormat format, VkExtent3D extent, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples,
			VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices,
			VkImageLayout initialLayout, PhysicalDevice physicalDevice, VkImageCreateFlags flags);
		/**
		* \brief Creates a ShaderModule
		* \param shaderFilePath The path to the Shader Code
		*/
		std::shared_ptr<ShaderModule> createShaderModule(std::string shaderFilePath);
		/**
		* \brief Creates a Renderpass with given Parameters. See the Vulkan Documentation for further Information.
		* \param attachmentDescriptions A vector of VkAttachmentDescription which describe the Attachments.
		* \param subPassDescription A vector of VkSubpassDescription, which describe the Subpasses.
		* \param subpassDependencies A vector of VkSubpassDependency which describe the dependencies between subpasses.
		*/
		std::shared_ptr<RenderPass> createRenderpass(std::vector<VkAttachmentDescription> attachmentDescriptions = {},
			std::vector<VkSubpassDescription> subPassDescription = {}, std::vector<VkSubpassDependency> subpassDependencies = {});
		/**
		* \brief Initializes the Vulkan PipelineLayout with given Parameters
		* \param setLayouts An Array of DeskriptorSetLayouts
		* \param pushConstantRanges The push Constants which can be accessed by the Pipeline
		* \see Device DeskriptorSet
		*/
		std::shared_ptr<PipelineLayout> createPipelineLayout(std::vector<std::shared_ptr<DeskriptorSetLayout>> setLayouts = {}, std::vector<VkPushConstantRange> pushConstantRanges = {});
		/**
		* \brief Used to create a Pipeline. 
		* \param pipelineLayout The Pipelinelayout which describes the Pipeline
		* \param renderpass The renderpass which should be used with this Pipeline
		* \param subpass The index of the subpass in the renderpass which should be used
		* \param shaderstages The amount of ShaderStages.
		* \param vertexInputStateCreateInfo A Pointer to a PipelineVertexInputStateCreateInfo.
		* \param inputAssemblyStateCreateInfo A pointer to a PipelineInputAssemblyStateCreateInfo, which describes the pipelinebehaviour at the input assembly state.
		* \param rasterizationStateCreateInfo A pointer to a PipelineRasterizationStateCreatInfo, which describes the pipelinebehaviour at the rasterization state.
		* \param viewportStateCreateInfo A pointer to a PipelineViewportStateCreateInfo, ignored if rasterizer is disabled.
		* \param multisampleinfo A pointer to a PipelineMultisampleStateCreateInfo, ignored if rasterizer is disabled .
		* \param colorBlendStateCreateInfo A pointer to a PipelineColorBlendStateCreateInfo, ignored if rasterizer is disabled or subpass has no support.
		* \param depthStencilCreateInfo A pointer to a PipelineDepthStencilStateCreateInfo, which is used for depth/stencil testing. Ignored if the Pipeline has rasterization disables or the subpass doesnt support depth/stencil testing.
		* \param tessellationStateCreateInfo A pointer to a VkPipelineTessellationStateCreateInfo, which describes if a Tesselationshader should be used and if so, how.
		* \param dynamicStateCreateInfo A pointer to a VkPipelineDynamicStateCreateInfo, which describes which properties of the pipeline are dynamic and can be changed independently of the pipeline state.
		* \param basePipelineHandle A pipeline to derive from.
		* \param basePipelineIndex The index of the basepipeline Handle creatinfo which should be used.
		* \param pipelineCache The PipelineCache which can be used for creation.
		* \return A shared pointer to a Pipeline.
		*/
		std::shared_ptr<Pipeline> createPipeline(
			std::shared_ptr<PipelineLayout> pipelineLayout,
			std::shared_ptr<RenderPass> renderpass,
			uint32_t subpass,

			std::vector<PipelineShaderStageCreateInfo> shaderstages = {},
			PipelineVertexInputStateCreateInfo* vertexInputStateCreateInfo = nullptr,
			PipelineInputAssemblyStateCreateInfo* inputAssemblyStateCreateInfo = nullptr,
			PipelineRasterizationStateCreateInfo* rasterizationStateCreateInfo = nullptr,
			PipelineViewportStateCreateInfo* viewportStateCreateInfo = nullptr,
			PipelineMultisampleStateCreateInfo* multisampleinfo = nullptr,
			PipelineColorBlendStateCreateInfo* colorBlendStateCreateInfo = nullptr,
			PipelineDepthStencilStateCreateInfo* depthStencilCreateInfo = nullptr,
			VkPipelineTessellationStateCreateInfo* tessellationStateCreateInfo = nullptr,
			VkPipelineDynamicStateCreateInfo* dynamicStateCreateInfo = nullptr,
			VkPipeline basePipelineHandle = VK_NULL_HANDLE,
			uint32_t basePipelineIndex = -1,
			VkPipelineCache pipelineCache = VK_NULL_HANDLE);

		/**
		* \brief Creates a Framebuffer with given Values.
		* \param renderpass The Renderpass to this Framebuffer
		* \param attachments The attachments the Framebuffer should have( i.e. ImageView, DepthView )
		* \param extents Define the Dimensions of the Renderpass
		* \param layers Used for Multiview. Set to 1 for no MultiviewSupport.
		* \return A Shared pointer to a Framebuffer
		* \see RenderPass ImageView
		*/
		std::shared_ptr<Framebuffer> createFramebuffer(std::shared_ptr<RenderPass> renderpass,
			std::vector<std::shared_ptr<ImageView>> attachments, VkExtent2D const& extents, uint32_t layers);
		/**
		* \brief Creates a CommandPool by this Device
		* \param queueFamilyIndex The Queue Family which should be used for this CommandPool.
		* \param flags Flag which defines the usage of this Poll and its Buffers.
		* \return A Shared pointer to a CommandPool
		* \see CommandPool CommandBuffer
		*/
		std::shared_ptr<CommandPool> createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
		/**
		* \brief Creates a Queue
		* \param familyIndex The QueueFamily the Queue belongs to.
		* \param queueIndex  The index within the QueueFamily of the Queue.
		* \return A Shared pointer to a Queue
		* \see Queue
		*/
		std::shared_ptr<Queue> createQueue(uint32_t familyIndex, uint32_t queueIndex);
		/**
		* \brief Creates a general Buffer.
		* \param physicalDeviceMemoryProperties The Memory properties of the Device.
		* \param size The size of the Buffer in Bytes
		* \param flags Defines how this Buffer should be used, i.e. VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, possible flags are listed in the Vulkan API
		* \param sharingMode Defines how this buffer can be accessed from queues. Possible flags are VK_SHARING_MODE_EXCLUSIVE and VK_SHARING_MODE_CONCURRENT
		* \param queueFamilyIndices All Queue Families which should have access to this Buffer
		* \param memoryPropertyflags The visibility of this memorychunk
		* \return A Shared pointer to a Buffer
		* \see Buffer
		*/
		std::shared_ptr<Buffer> createBuffer(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
			VkDeviceSize size, VkBufferUsageFlags flags, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices, VkMemoryPropertyFlags memoryPropertyflags);
		/**
		* \brief Creates a Shared pointer to a DeskriptorSetLayout.
		* \param bindings
		* \return A Shared pointer to a DeskriptorSetlayout
		* \see DeskriptorSetLayout
		*/
		std::shared_ptr<DeskriptorSetLayout>  createDeskriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings);
		/**
		* \brief Creates a DeskriptorPool.
		* \param sets The maximum amount of DescriptorSets which can be created by this CommandPool
		* \param poolSizes An Array of the VkDescriptorPoolSize Structure, which contains a Description and the amount of Descriptors which should be allocated by that pool
		* \return A Shared pointer to a DeskriptorPool
		* \see DeskriptorPool DescriptorSet
		*/
		std::shared_ptr<DeskriptorPool> createDeskriptorPool(uint32_t sets, std::vector<VkDescriptorPoolSize> poolSizes, VkDescriptorPoolCreateFlags flag);
		/**
		* \brief Creates a Vulkan DescriptorSet.
		* \param pool The corresponding CommandPool to this DescriptorSet
		* \param layout The Layout which define how the DescriptorSet should be allocated
		* \return A Shared pointer to a DeskriptorSet
		* \see DeskriptorPool DeskriptorSet DeskriptorSetLayout
		*/
		std::shared_ptr<DeskriptorSet> createDeskriptorSet(std::shared_ptr<DeskriptorPool> pool, std::shared_ptr<DeskriptorSetLayout> layout);
		/**
		* \brief Creates a Semaphore.
		* \return A Shared pointer to a Semaphore
		*/
		std::shared_ptr<Semaphore> createSemaphore();
		/**
		* \brief Used to create a Vulkan Sampler with given Parameters.
		* \param magFilter Specifies the magnification filter to apply to lookups.
		* \param minFilter  Specifies the minification filter to apply to lookups.
		* \param mipmapMode Specifies the mipmap filter to apply to lookups.
		* \param addressModeU specifies addressing mode outside of 0 and 1 range for U coordinate
		* \param addressModeV specifies addressing mode outside of 0 and 1 range for V coordinate
		* \param addressModeW specifies addressing mode outside of 0 and 1 range for W coordinate
		* \param mipLodBias The Bias to be added to LoD calculations
		* \param anisotropyEnable Enables or Disables anisotropic filtering
		* \param maxAnisotropy The anisotropy value which should be used if anisotropyEnable is true
		* \param compareEnable Enables or Disables comparison against a reference for lookups
		* \param compareOp The value to use for comparison
		* \param minLod minValue to clamp for LoD
		* \param maxLod maxValue to clamp for LoD
		* \param borderColor Color to use for the border
		* \param unnormalizedCoordinates Defines if Texel Coordinates should be normalized or not
		* \return A Shared pointer to a Sampler.
		*/
		std::shared_ptr<Sampler> createSampler(VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
			VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, float mipLodBias, bool anisotropyEnable, float maxAnisotropy, bool compareEnable,
			VkCompareOp compareOp, float minLod, float maxLod, VkBorderColor borderColor, bool unnormalizedCoordinates);
		/**
		* \brief Creates a 2D Texture. Currently only .jpg is supported
		* \param textureFilePath The filepath to the Texture
		* \param queueFamilyIndex The queuefamily who has access to this Texture
		* \param queue The Queue which has to do the submitting
		* \return A Shared Pointer to a (2D)Texture
		*/
		std::shared_ptr<Texture>create2DTexture(std::string textureFilePath, std::shared_ptr<Queue> queue);
		/**
		* \brief Creates a Cubemap Texture. Currently only .ktx is supported
		* \param textureFilePath The filepath to the Texture
		* \param queueFamilyIndex The queuefamily who has access to this Texture
		* \param queue The Queue which has to do the submitting
		* \return A Shared Pointer to a (Cubemap)Texture
		*/
		std::shared_ptr<Texture> Device::createCubemapTexture(std::string textureFilePath, std::shared_ptr<Queue> queue);
		/**
		* \brief Getter for the Vulkan Device Handle.
		* \return The Vulkan Handle of this device
		*/
		VkDevice getDevice() const { return device; }
		/**
		* \brief Getter for the PhysicalDevice of this logical Device.
		* \return The PhysicalDevice of this device
		*/
		std::shared_ptr<PhysicalDevice> getPhysicalDevice() const { return physicalDevice; }
	private:

		VkDevice device;
		std::shared_ptr<VkInstance> instance;
		std::shared_ptr<PhysicalDevice> physicalDevice;
		std::vector<std::string> deviceExtensionNames;
		std::vector<std::string> layerNames;
		std::vector<VkDeviceQueueCreateInfo> m_deviceQueueCreateInfos;
		//std::vector<std::shared_ptr<Queue>> queues; //@TODO create and save all queues automatically
	};

}
