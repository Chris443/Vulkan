#pragma once

#include "Vertex.h"
#include "Texture.h"
#include "tiny_obj_loader.h"

namespace vkf {
	class Buffer;
	class Device;
	class Queue;
	class CommandPool;

	struct material {
		std::vector<glm::vec3> materials; // 0 = ambient, 1 = diffuse, 2 = specular /@TODO: add diffuse/specular maps
	};

	/**
	* \brief Class to Load a .obj mesh
	*/
	class Mesh {
	public:
		Mesh() {};
		/**
		* \brief Loads a .obj Mesh from given filepath
		* \param path to the .obj file
		* \param logical device to this class
		* \param queue Queue to use for loading the Data into Buffers
		* \param cmdPool The commandpool, which is needed for commandbuffer creation
		*/
		void loadMesh(const char*path, std::shared_ptr<Device> device, std::shared_ptr<Queue> queue, std::shared_ptr<CommandPool> cmdPool);
		/**
		* \brief Getter for mesh's vertices
		* \return Vector of vertices
		*/
		std::vector<Vertex> getVertices() { return vertices; };
		/**
		* \brief Getter for mesh's indices
		* \return Vector of indices
		*/
		std::vector<uint32_t> getIndices() { return indices; };
		/**
		* \brief Getter for the vertexbuffer of this mesh
		* \return shared_ptr of the vertexbuffer
		*/
		std::shared_ptr<Buffer> getVertexBuffer() { return vertexBuffer; };
		/**
		* \brief Getter for the indexbuffer of this mesh
		* \return shared_ptr of the indexbuffer
		*/
		std::shared_ptr<Buffer> getIndexBuffer() { return indexBuffer; };
		material getMaterial() { return material; }
	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::shared_ptr<Buffer> vertexBuffer;
		std::shared_ptr<Buffer> indexBuffer;
		material material;

	};
}
