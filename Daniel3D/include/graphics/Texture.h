#pragma once

#include <glad/glad.h>
#include <string>

namespace dg3d
{
	namespace graphics
	{
		class Texture
		{
		public:
			Texture(GLuint ID, int w, int h);
			~Texture();

			const GLuint mID;

			const int mWidth;
			const int mHeight;

			static Texture* Create(const std::string& filepath);
			static void Destroy(Texture* texture);
		};
	}
}
