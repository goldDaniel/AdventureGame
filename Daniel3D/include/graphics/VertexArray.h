#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

namespace dg3d
{
	namespace graphics
	{
		class VertexArray
		{
		private:
			uint32_t mInstanceCount;
			GLuint mID = 0;

			uint32_t mVertexBufferIndex = 0;
			std::vector<GLuint> mVertexBuffers;

		public:
			VertexArray();
			~VertexArray();

			void Bind() const;
			void Unbind() const;

			void SetNumInstances(uint32_t instances);
			uint32_t GetInstanceCount() const;

			void AddVertexBuffer(const std::vector<float>& vertexBuffer, uint32_t elementsPerVertex);
			void AddInstancedVertexBuffer(std::vector<float>& vertexBuffer, uint32_t elementsPerVertex, uint32_t attribDivisor);
			void AddInstancedVertexBuffer(const std::vector<glm::mat4>& matrices);

			uint32_t GetID() const;
		};
	}
}

