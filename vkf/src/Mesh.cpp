#pragma once

#include "../Mesh.h"
#include "../Buffer.h"
#include "../Device.h"
#include "../Queue.h"
#include "../CommandPool.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"


namespace vkf {
	void Mesh::loadMesh(const char*path, std::shared_ptr<Device> device, std::shared_ptr<Queue> queue, std::shared_ptr<CommandPool> cmdPool)
	{
		tinyobj::attrib_t vertexAttributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string errorString;

		std::string mtlpath = path;
		uint32_t s = mtlpath.find_last_of('/', 100);
		mtlpath.erase(mtlpath.begin() + s + 1, mtlpath.end());

		bool success = tinyobj::LoadObj(&vertexAttributes, &shapes, &materials, &errorString, path, mtlpath.c_str());
		if (!success) {
			throw std::runtime_error(errorString);
		}
		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

		//Get Data
		for (tinyobj::shape_t shape : shapes) {
			for (tinyobj::index_t index : shape.mesh.indices) {
				glm::vec3 pos = {
					vertexAttributes.vertices[3 * index.vertex_index + 0],
					vertexAttributes.vertices[3 * index.vertex_index + 1],
					vertexAttributes.vertices[3 * index.vertex_index + 2]
				};

				glm::vec3 nor = {
					vertexAttributes.normals[3 * index.normal_index + 0],
					vertexAttributes.normals[3 * index.normal_index + 1],
					vertexAttributes.normals[3 * index.normal_index + 2]
				};

				glm::vec2 tex = {
					vertexAttributes.texcoords[2 * index.texcoord_index + 0],
					1.0f - vertexAttributes.texcoords[2 * index.texcoord_index + 1]
				};

				Vertex vert(pos, tex, nor);

				if (uniqueVertices.count(vert) == 0) {
					uniqueVertices[vert] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vert);
				}

				indices.push_back(uniqueVertices[vert]);

			}
			if (materials.size() > 0) {
				vkf::material m;
				auto amb = materials[shape.mesh.material_ids[s]].ambient;
				auto diff = materials[shape.mesh.material_ids[s]].diffuse;
				auto spec = materials[shape.mesh.material_ids[s]].specular;

				this->material.materials.push_back(glm::vec3(amb[0], amb[1], amb[2]));
				this->material.materials.push_back(glm::vec3(diff[0], diff[1], diff[2]));
				this->material.materials.push_back(glm::vec3(spec[0], spec[1], spec[2]));
			}
		}
		std::shared_ptr<Buffer> stagingBuffer = device->createBuffer(device->getPhysicalDevice()->getMemoryProperties(), sizeof(Vertex)*vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

		stagingBuffer->map(vertices.data());

		vertexBuffer = device->createBuffer(device->getPhysicalDevice()->getMemoryProperties(), sizeof(Vertex)*vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		vertexBuffer->map(vertices.data());

		//buffers for stage Buffers
		std::shared_ptr<CommandBuffer> transferBuffer = cmdPool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		transferBuffer->beginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		transferBuffer->copy(stagingBuffer, 0, 0, vertexBuffer, sizeof(Vertex)*vertices.size());
		transferBuffer->endRecording();

		vkf::SubmitInfo stageBufferSubmitInfo({}, { transferBuffer }, {}, {});
		queue->submit({ stageBufferSubmitInfo });
		queue->wait();

		indexBuffer = device->createBuffer(device->getPhysicalDevice()->getMemoryProperties(), sizeof(uint32_t)*indices.size(),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

		indexBuffer->map(indices.data());
	}
}