#include "../CommandBuffer.h"
#include "../Buffer.h"
#include "../PipelineLayout.h"
#include "../DeskriptorSet.h"
#include "../Pipeline.h"
#include "../RenderPass.h"
#include "../Framebuffer.h"
#include "../Image.h"

namespace vkf {

	CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool cmdPool, VkCommandBufferLevel level)
	:device(device),
	pool(cmdPool){
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = NULL;
		allocInfo.commandPool = cmdPool;
		allocInfo.level = level;
		allocInfo.commandBufferCount = 1;

		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
	}

	void CommandBuffer::beginRecording(VkCommandBufferUsageFlags flags,
		VkCommandBufferInheritanceInfo*  pInheritanceInfo) {

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.flags = flags;
		beginInfo.pInheritanceInfo = pInheritanceInfo;

		VkResult res = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		assert(!res);
	}


	void CommandBuffer::endRecording() {
		VkResult res = vkEndCommandBuffer(commandBuffer);
		assert(!res);
	}

	void CommandBuffer::beginRenderPass(VkRenderPassBeginInfo renderpassInfo, VkSubpassContents contents) {
		vkCmdBeginRenderPass(commandBuffer, &renderpassInfo, contents);
	}

	void CommandBuffer::CommandBuffer::beginRenderPass(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<Framebuffer> framebuffer, VkRect2D const& area, std::vector<VkClearValue> clearValues, VkSubpassContents contents) {
		VkRenderPassBeginInfo renderpassInfo = {};
		renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderpassInfo.pNext = 0;
		renderpassInfo.renderPass = renderPass->getRenderpass();
		renderpassInfo.framebuffer = framebuffer->getFramebuffer();
		renderpassInfo.renderArea = area;
		renderpassInfo.clearValueCount = clearValues.size();
		renderpassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderpassInfo, contents);
	}

	void CommandBuffer::endRenderPass() {
		vkCmdEndRenderPass(commandBuffer);
	}

	void CommandBuffer::bindPipeline(std::shared_ptr<Pipeline> pipeline, VkPipelineBindPoint  pipelineBindPoint) {
		vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline->getPipeline());
	}

	void CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,uint32_t firstInstance) {
		vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::drawIndexed(int32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance){
		vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CommandBuffer::copy(std::shared_ptr<Buffer> src,VkDeviceSize srcOffset,VkDeviceSize dstOffset, std::shared_ptr<Buffer> dest,VkDeviceSize size) {
		VkBufferCopy bufferCopy;
		bufferCopy.srcOffset = srcOffset;
		bufferCopy.dstOffset = dstOffset;
		bufferCopy.size = size;
		vkCmdCopyBuffer(commandBuffer, src->getBuffer(), dest->getBuffer(), 1, &bufferCopy);
	}

	void CommandBuffer::bindVertexBuffer(uint32_t binding,uint32_t bindingCount, std::vector<std::shared_ptr<Buffer>> buffers, std::vector<VkDeviceSize> offsets ) {
		std::vector<VkBuffer> bfrs;
		for (uint32_t i = 0; i < buffers.size(); ++i) {
			bfrs.push_back(buffers[i]->getBuffer());
		}
		vkCmdBindVertexBuffers(commandBuffer, binding, bindingCount, bfrs.data(), offsets.data());
	}

	void CommandBuffer::bindIndexBuffer(std::shared_ptr<Buffer> buffer, VkDeviceSize offset, VkIndexType indexType) {
		vkCmdBindIndexBuffer(commandBuffer, buffer->getBuffer(), offset, indexType);
	}

	void CommandBuffer::bindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, std::shared_ptr<PipelineLayout> pipelineLayout, uint32_t firstSet,
		std::vector<std::shared_ptr<DeskriptorSet>> descriptorSets, std::vector<uint32_t> const& dynamicOffsets) {
		std::vector<VkDescriptorSet> bindDescriptorSets;
		for (auto d : descriptorSets) {
			bindDescriptorSets.push_back(d->getDescriptorSet());
		}
		vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, pipelineLayout->getPipelineLayout(), firstSet, descriptorSets.size(), bindDescriptorSets.data(), dynamicOffsets.size(), dynamicOffsets.data());
	}

	void CommandBuffer::copyBufferToImage(std::shared_ptr<Buffer> srcBuffer, std::shared_ptr<Image> image, VkImageLayout dstImageLayout,std::vector<VkBufferImageCopy> regions) {
		vkCmdCopyBufferToImage(commandBuffer, srcBuffer->getBuffer(), image->getImage(), dstImageLayout, regions.size(), regions.data());
	}

	void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags destStageMask, VkDependencyFlags flags, std::vector<VkMemoryBarrier> memoryBarriers,
		std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers,std::vector<VkImageMemoryBarrier> imageMemoryBarriers) {
		vkCmdPipelineBarrier(commandBuffer, srcStageMask, destStageMask, flags, memoryBarriers.size(), memoryBarriers.data(), bufferMemoryBarriers.size(), bufferMemoryBarriers.data(), imageMemoryBarriers.size(),
			imageMemoryBarriers.data());
	}

	void CommandBuffer::reset(VkCommandBufferResetFlags flags) {
		vkResetCommandBuffer(commandBuffer, flags);
	}
}
