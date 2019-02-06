#pragma once
#include "common.h"

namespace vkf {
	class Buffer;
	class DeskriptorSet;
	class PipelineLayout;
	class Pipeline;
	class RenderPass;
	class Framebuffer;
	class Image;

	/**
	* \brief The CommandBuffer class encapsulates a Vulkan CommandBuffer Handle and takes care of initialization and deleting. CommandBuffers can be created/allocated by the CommandPool. Use the allocateBuffer() function inside the CommandPool class.
	* This class provides many functionwrappers to help for easy and fast usage of Vulkan CommandBuffers.
	*/
	class CommandBuffer {
	public:
		/**
		* \brief Constructor for creating a Vulkan CommandBuffer. Use the CommandPool.allocateBuffer() function instead of the constructor.
		* \param device The logical device to which this CommandBuffer belongs.
		* \param cmdPool The corresponding CommandPool.
		* \param level The Level of this CommandBuffer, i.e. if it's a primary of secondary Buffer.
		*/
		CommandBuffer(VkDevice device, VkCommandPool cmdPool, VkCommandBufferLevel level);
		/**
		* \brief Start every recording with this function. Fills a VkCommandBufferBeginInfo and starts recording by calling vkBeginCommandBuffer.
		* \param flags specifies the usage behavior of this commandBuffer, i.e. VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT is used for Buffers which are consumed once. Most times you will want to use VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
		* \param pInheritanceInfo Used for secondary CommandBuffers, ignored for Primary ones.
		*/
		void beginRecording(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			VkCommandBufferInheritanceInfo* pInheritanceInfo = nullptr);
		/**
		* \brief Ends Recording. Always call this after recordings.
		*/
		void endRecording();
		/**
		* \brief Overloaded Function for custom filled RenderpassBeginInfo. Begins a renderPass. You have to call this Function if you want a Renderpass to be executed with this CommandBuffer.
		* \param renderpassBeginInfo The RenderPassBeginInfo for starting this Renderpass.
		* \param contents How the Commands will be provided. For Primary-only Buffer VK_SUBPASS_CONTENTS_INLINE is used as default value.
		*/
		void beginRenderPass(VkRenderPassBeginInfo renderpassBeginInfo, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
		/**
		* \brief Begins a renderPass. You have to call this Function if you want a Renderpass to be executed with this CommandBuffer.
		* \param renderPass The corresponding Renderpass which should be executed with this CommandBuffer
		* \param framebuffer The Framebuffer which should be associated with the Renderpass
		* \param area The Area which is affected by the Renderpass
		* \param clearValues The Color(s) with which the Window should be cleared
		* \param contents How the Commands will be provided. For Primary-only Buffer VK_SUBPASS_CONTENTS_INLINE is used as default value.
		*/
		void beginRenderPass(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<Framebuffer> framebuffer, VkRect2D const& area, std::vector<VkClearValue> clearValues, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
		/**
		* \brief Ends the Renderpass to this commandBuffer.
		*/
		void endRenderPass();
		/**
		* \brief Binds the given pipeline to this CommandBuffer
		* \param pipeline The Pipeline to bind by this CommandBuffer
		* \param pipelineBindPoint Bind Pipeline as Graphics or Compute Pipeline
		*/
		void bindPipeline(std::shared_ptr<Pipeline> pipeline, VkPipelineBindPoint  pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		/**
		* \brief Binds given (Vertex)Buffer to this CommandBuffer.
		* \param binding The Index of the First Input binding whose state is updated by the CommandBuffer.(See VkVertexInputBindingDescription)
		* \param bindingCount Amount of VertexInputBindings which should be updated
		* \param buffers Array of Buffers which should be used
		* \param offsets Array of Offsets
		*/
		void bindVertexBuffer(uint32_t binding, uint32_t bindingCount, std::vector<std::shared_ptr<Buffer>> buffers, std::vector<VkDeviceSize> offsets);
		/**
		* \brief binds given Buffer as IndexBuffer.
		* \param buffer The buffer which should be Bound
		* \param offset The offset in Bytes within the Buffer
		* \param indexType Defines if the Indices should be read as 16 or 32 bits. Possible Values are VK_INDEX_TYPE_UINT16 and VK_INDEX_TYPE_UINT32
		*/																							   
		void bindIndexBuffer(std::shared_ptr<Buffer> buffer, VkDeviceSize offset, VkIndexType indexType);
		/**
		* \brief Binds given DescriptorSets.
		* \param pipelineBindPoint Specifies if Descriptor will be used in Graphics or Compute Pipeline
		* \param pipelineLayout The Layout the Descriptors are based on
		* \param firstSet Index of the First Set
		* \param descriptorSets The DescriptorSets which should be bind
		* \param dynamicOffsets Specifies the offset
		*/
		void bindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, std::shared_ptr<PipelineLayout> pipelineLayout, uint32_t firstSet,
			std::vector<std::shared_ptr<DeskriptorSet>> descriptorSets, std::vector<uint32_t> const& dynamicOffsets);
		/**
		* \brief Copies the content of a Buffer into an Image.
		* \param srcBuffer The source Buffer, which contents should be copied.
		* \param image The destination Image, which should be filled with the Buffer Data.
		* \param dstImageLayout The Image layout of the destination Image.
		* \param regions Defines the Region which should be copied
		*/
		void copyBufferToImage(std::shared_ptr<Buffer> srcBuffer, std::shared_ptr<Image> image, VkImageLayout dstImageLayout ,std::vector<VkBufferImageCopy> regions);
		/**
		* \brief Function to start non-indexed drawing.
		* \param vertexCount The amount of Vertices which should be drawn
		* \param instanceCount Used for Instanced rendering. If you are not Instancedrendering, set this to 1.
		* \param firstVertex Offset, which specifies which Vertex should be drawn first.
		* \param firstInstance Offset, which specifies which Instance should be drawn first.
		*/
		void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		/**
		* \brief Function for indexed Drawing
		* \param indexCount The Number of Vertices
		* \param instanceCount The Number of Instances
		* \param firstIndex First Index of the Index Buffer
		* \param vertexOffset The Offset inside the Index buffer
		* \param firstInstance The Index of the First Instance
		*/
		void drawIndexed(int32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
		/**
		* \brief Function to Copy data from one Buffer to another.
		* \param src The Source Buffer, from which should be copied
		* \param srcOffset The offset inside the Source Buffer
		* \param dstOffset the offset inside the Destination buffer
		* \param dest The Destination buffer, which should be filles with the data.
		* \param region The Region which Specifies what should be copies
		*/
		void copy(std::shared_ptr<Buffer> src, VkDeviceSize srcOffset, VkDeviceSize dstOffset,std::shared_ptr<Buffer> dest, VkDeviceSize region);
		/**
		* \brief Used to synchronize access to resources(Buffers and Images). Use bufferMemoryBarriers for buffers and imageMemoryBarriers for images.
		* \param srcStageMask Specifies on which Stage the Operations should happen before the barrier
		* \param destStageMask Specifies where operations will wait for the barrier
		* \param flags Specifies Dependencies
		* \param memoryBarriers The MemoryBarriers on which should be waited
		* \param bufferMemoryBarriers The bufferMemoryBarriers on which should be waited
		* \param imageMemoryBarriers The imageMemoryBarriers on which should be waited
		*/
		void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags destStageMask, VkDependencyFlags flags, std::vector<VkMemoryBarrier> memoryBarriers,
			std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers, std::vector<VkImageMemoryBarrier> imageMemoryBarriers);
		/**
		* \brief Resets this CommandBuffer and releases it's resources
		* \param flags Defines how this Commandbuffer should be reset. Currently only VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT is a valid value.
		*/
		void reset(VkCommandBufferResetFlags flags);
		/**
		* \brief returns the Vulkan CommandBuffer Handles.
		* \return the CommandBuffer Handle 
		*/
		VkCommandBuffer getCommandBuffer() { return commandBuffer; };

	protected:
		friend class CommandPool;
	private:
		/**
		* The corresponding Vulkan Device Handle
		*/
		VkDevice device;
		/**
		* The Vulkan Handle of this CommandBuffer
		*/
		VkCommandBuffer commandBuffer;
		/**
		* The corresponding commandPool Handle of this CommandBuffer
		*/
		VkCommandPool pool;
	};

}