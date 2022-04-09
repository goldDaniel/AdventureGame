#pragma once

#include <cstdint>
#include <vector>
#include <glad/glad.h>


namespace dg3d
{
	namespace graphics
	{
		class IndexBuffer
		{
		private:
			GLuint mID = 0;
			GLuint mElementCount = 0;

		public:
			IndexBuffer(const std::vector<GLuint>& indices);
			~IndexBuffer();

			void Bind() const;

			GLuint GetID() const;
			GLuint GetElementCount() const;
		};
	}
}