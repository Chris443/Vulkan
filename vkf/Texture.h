#pragma once
#include "common.h"


namespace vkf {
	class Buffer;
	class CommandPool;
	class CommandBuffer;
	class Queue;
	class Device;
	class Image;
	class ImageView;
	class Sampler;
	enum TextureType {
		D2,
		CUBEMAP
	};
	/**
	* \brief Class to load different kinds of Textures. Used inside the Device class
	*/

	class Texture
	{
	public:
		/**
		* \brief Loads a Texture from given filepath
		* \param device Device to load this Texture off
		* \param textureFilePath The filePath to the Texture
		* \param queue The queue to load this file
		* \param type The Type of Texture to load. Currently 2D Textures and Cubemaps are supported
		*/
		Texture(std::shared_ptr<Device> device, std::string textureFilePath, std::shared_ptr<Queue> queue, TextureType type);
		/**
		* \brief returns the Image of this Texture
		* \return A Shared Pointer of the Image
		*/
		std::shared_ptr<Image> getImage() { return textureImage; };
		/**
		* \brief returns the ImageView of this Texture
		* \return A Shared Pointer of the ImageView
		*/
		std::shared_ptr<ImageView> getImageView() { return textureImageView; };
		/**
		* \brief returns the Sampler of this Texture
		* \return A Shared Pointer of the Sampler
		*/
		std::shared_ptr<Sampler> getSampler() { return textureSampler; };
	private:
		int texWidth, texHeight, texChannels, mipLevels;
		std::shared_ptr<Image> textureImage;
		std::shared_ptr<ImageView> textureImageView;
		std::shared_ptr<Sampler> textureSampler;

		void load2DTex(std::shared_ptr<Device> device, std::string textureFilePath, std::shared_ptr<Queue> queue);
		void loadCubeMap(std::shared_ptr<Device> device, std::string textureFilePath, std::shared_ptr<Queue> queue);
	};
}