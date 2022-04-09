#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace dg3d
{
	namespace graphics
	{
		class Texture;
		class TextureRegion;
		class Shader;

		class SpriteBatch
		{
		private:
			const size_t mMaxSprites;

			GLuint mVertexArray = 0;

			GLuint mPositionVBO = 0;
			GLuint mColorVBO = 0;
			GLuint mTexCoordVBO = 0;
			GLuint mEBO = 0;

			std::vector<glm::vec3> mPositionBuffer;
			std::vector<glm::vec3> mColorBuffer;
			std::vector<glm::vec2> mTexCoordBuffer;
			std::vector<unsigned int> mIndicesBuffer;

			glm::mat4 mProj = glm::mat4(1.0f);
			glm::mat4 mView = glm::mat4(1.0f);

			glm::vec3 mCurrentColor = { 1, 1, 1 };

			bool mIsRunning = false;

			Texture const* mCurrentTexture;

			std::unique_ptr<Shader> shader;

		public:
			SpriteBatch(const size_t maxSprites = 4096);
			~SpriteBatch();

			void SetColor(const glm::vec4& color);

			void Begin(const glm::mat4& proj, const glm::mat4& view);
			void End();

			void Draw(TextureRegion const* tex, float x, float y, float width, float height);
			void Draw(TextureRegion const* tex, float x, float y, float width, float height, float angle);
			void Draw(Texture const* tex, float x, float y, float width, float height);
			void Draw(Texture const* tex, float x, float y, float width, float height, float angle);
			void Draw(Texture const* tex, float x, float y, float width, float height, float angle, glm::vec2 uvMin, glm::vec2 uvMax);

		private:
			void Clear();
			void Flush();
		
		};
	}
}