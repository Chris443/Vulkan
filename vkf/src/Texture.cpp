#include "../Texture.h"
#include "../Buffer.h"
#include "../CommandPool.h"
#include "../CommandBuffer.h"
#include "../Queue.h"
#include "../Device.h"
#include "../Image.h"
#include "../ImageView.h"
#include "../Sampler.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"


namespace vkf {

	Texture::Texture(std::shared_ptr<Device> device, std::string textureFilePath, std::shared_ptr<Queue> queue,TextureType type)
	{
		if (type == TextureType::D2) {
			load2DTex(device, textureFilePath, queue);
		}
		else if (type == TextureType::CUBEMAP) {
			loadCubeMap(device, textureFilePath, queue);
			}
	}


	void Texture::load2DTex(std::shared_ptr<Device> device, std::string textureFilePath, std::shared_ptr<Queue> queue) {

		stbi_uc* pixels = stbi_load(textureFilePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}




		std::shared_ptr<Buffer> textureStagingBuffer = device->createBuffer(device->getPhysicalDevice()->getMemoryProperties(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, {},
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		textureStagingBuffer->map(pixels);

		stbi_image_free(pixels);

		textureImage = device->createImage(VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, { (uint32_t)texWidth,(uint32_t)texHeight,1 }, 1, 1,
			VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_SHARING_MODE_EXCLUSIVE,
			{}, VK_IMAGE_LAYOUT_UNDEFINED, *device->getPhysicalDevice(),0);

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { (uint32_t)texWidth,	(uint32_t)texHeight,	1 };

		//single-use commandbuffer
		std::shared_ptr<CommandPool> pool = device->createCommandPool(queue->getFamilyIndex());
		std::shared_ptr<vkf::CommandBuffer> cmdbuff = pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		cmdbuff->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Driver Optimization

																			  //Layout Transition
		VkImageMemoryBarrier imageBarrier = textureImage->getImageMemoryBarrier(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT);
		cmdbuff->pipelineBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {}, { imageBarrier });

		cmdbuff->endRecording();
		vkf::SubmitInfo submitInfo({}, { cmdbuff }, {}, {});
		queue->submit({ submitInfo });
		queue->wait();

		//single-use commandbuffer
		cmdbuff = pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		cmdbuff->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Driver Optimization

		cmdbuff->copyBufferToImage(textureStagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, { region });

		cmdbuff->endRecording();
		submitInfo = vkf::SubmitInfo({}, { cmdbuff }, {}, {});
		queue->submit({ submitInfo });
		queue->wait();

		//single-use commandbuffer
		cmdbuff = pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		cmdbuff->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Driver Optimization

		imageBarrier = textureImage->getImageMemoryBarrier(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
		cmdbuff->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, {}, {}, { imageBarrier });

		cmdbuff->endRecording();
		submitInfo = vkf::SubmitInfo({}, { cmdbuff }, {}, {});
		queue->submit({ submitInfo });
		queue->wait();

		//ImageView
		textureImageView = textureImage->createImageView(VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM);

		//textureSampler
		textureSampler = device->createSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT,
			VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, VK_TRUE, 16, VK_FALSE, VK_COMPARE_OP_ALWAYS, 0.0f, 0.0f, VK_BORDER_COLOR_INT_OPAQUE_BLACK, VK_FALSE);
	}
	void Texture::loadCubeMap(std::shared_ptr<Device> device, std::string textureFilePath, std::shared_ptr<Queue> queue) {
		stbi_uc* front = stbi_load((textureFilePath+ "/posz.jpg").c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		stbi_uc* back = stbi_load((textureFilePath+ "/negz.jpg").c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		stbi_uc* down = stbi_load((textureFilePath+ "/negy.jpg").c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		stbi_uc* top = stbi_load((textureFilePath+ "/posy.jpg").c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		stbi_uc* right = stbi_load((textureFilePath+ "/posx.jpg").c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		stbi_uc* left = stbi_load((textureFilePath+ "/negx.jpg").c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = texWidth * texHeight * 4 * 6;
		VkDeviceSize layerSize = imageSize / 6;


		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM; 


		std::shared_ptr<Buffer> textureStagingBuffer = device->createBuffer(device->getPhysicalDevice()->getMemoryProperties(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, {},
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* rawData;

		vkMapMemory(device->getDevice(), textureStagingBuffer->getMemory(), 0, imageSize, 0, &rawData);

		memcpy(rawData, right, (size_t)layerSize);

		memcpy((char*)rawData + layerSize , left, (size_t)layerSize);

		memcpy((char*)rawData + 2*layerSize, top, (size_t)layerSize);

		memcpy((char*)rawData + 3*layerSize, down, (size_t)layerSize);

		memcpy((char*)rawData + 4*layerSize, front, (size_t)layerSize);

		memcpy((char*)rawData + 5*layerSize, back, (size_t)layerSize);

		vkUnmapMemory(device->getDevice(), textureStagingBuffer->getMemory());


		textureImage = device->createImage(VK_IMAGE_TYPE_2D, format, { (uint32_t)texWidth,(uint32_t)texHeight,1 }, 1, 6,
			VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_SHARING_MODE_EXCLUSIVE,
			{}, VK_IMAGE_LAYOUT_UNDEFINED, *device->getPhysicalDevice(), VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);


		/****Copy****/

		//single-use commandbuffer
		std::shared_ptr<CommandPool> pool = device->createCommandPool(queue->getFamilyIndex());
		std::shared_ptr<vkf::CommandBuffer> cmdbuff = pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		cmdbuff->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Driver Optimization

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 6;		
		VkImageMemoryBarrier imageBarrier = textureImage->getImageMemoryBarrier(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT, subresourceRange);
		cmdbuff->pipelineBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {}, { imageBarrier });

		cmdbuff->endRecording();
		vkf::SubmitInfo submitInfo({}, { cmdbuff }, {}, {});
		queue->submit({ submitInfo });
		queue->wait();

		//single-use commandbuffer
		cmdbuff = pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		cmdbuff->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Driver Optimization

		std::vector<VkBufferImageCopy> bufferCopyRegions;
		uint32_t offset = 0;

		for (uint32_t face = 0; face < 6; face++)
		{
				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferCopyRegion.imageSubresource.mipLevel = 0;
				bufferCopyRegion.imageSubresource.baseArrayLayer = face;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent.width = texWidth;
				bufferCopyRegion.imageExtent.height = texHeight;
				bufferCopyRegion.imageExtent.depth = 1;
				bufferCopyRegion.bufferOffset = offset;

				bufferCopyRegions.push_back(bufferCopyRegion);

				// Increase offset into staging buffer for next level / face
				offset += (size_t)layerSize;
		}

		cmdbuff->copyBufferToImage(textureStagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, bufferCopyRegions);

		cmdbuff->endRecording();
		submitInfo = vkf::SubmitInfo({}, { cmdbuff }, {}, {});
		queue->submit({ submitInfo });
		queue->wait();

		//single-use commandbuffer
		cmdbuff = pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		cmdbuff->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Driver Optimization

		imageBarrier = textureImage->getImageMemoryBarrier(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
		cmdbuff->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, {}, {}, { imageBarrier });

		cmdbuff->endRecording();
		submitInfo = vkf::SubmitInfo({}, { cmdbuff }, {}, {});
		queue->submit({ submitInfo });
		queue->wait();

		/****Copy****/

		//ImageView
		textureImageView = textureImage->createImageView(VK_IMAGE_VIEW_TYPE_CUBE, format,
			{ VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A }, subresourceRange);

		//textureSampler
		textureSampler = device->createSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, VK_TRUE, 16, VK_FALSE, VK_COMPARE_OP_NEVER, 0.0f, 1, VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, VK_FALSE);
	}
}