#pragma once
#include "common.h"
#include "glm/glm.hpp"
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace vkf {

	struct Vertex {
	public:
		glm::vec3 pos;
		glm::vec2 texCoord;
		glm::vec3 normal;

		Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 normal)
			:pos(pos), texCoord(tex), normal(normal)
		{}

		bool operator==(const Vertex& other) const {
			return pos == other.pos  && texCoord == other.texCoord && normal == other.normal;
		}

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription vertexInputBindingDescription;
			vertexInputBindingDescription.binding = 0;
			vertexInputBindingDescription.stride = sizeof(Vertex);
			vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return vertexInputBindingDescription;
		}

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
			std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions(3);

			vertexInputAttributeDescriptions[0].binding = 0;
			vertexInputAttributeDescriptions[0].location = 0;
			vertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexInputAttributeDescriptions[0].offset = offsetof(Vertex, pos);

			vertexInputAttributeDescriptions[1].binding = 0;
			vertexInputAttributeDescriptions[1].location = 1;
			vertexInputAttributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
			vertexInputAttributeDescriptions[1].offset = offsetof(Vertex, texCoord);

			vertexInputAttributeDescriptions[2].binding = 0;
			vertexInputAttributeDescriptions[2].location = 2;
			vertexInputAttributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexInputAttributeDescriptions[2].offset = offsetof(Vertex, normal);

			return vertexInputAttributeDescriptions;
		}
	};

}

namespace std {
	template<> struct hash<vkf::Vertex> {
		size_t operator()(vkf::Vertex const& vertex) const {
			size_t h1 = hash<glm::vec3>()(vertex.pos);
			size_t h2 = hash<glm::vec2>()(vertex.texCoord);
			size_t h3 = hash<glm::vec3>()(vertex.normal);

			return ((h1 ^ (h2 << 1)) >> 1) ^ h3;
		}
	};
}