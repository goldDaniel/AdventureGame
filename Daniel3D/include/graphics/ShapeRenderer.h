#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <vector>

namespace dg3d
{
    namespace graphics
    {
        class ShapeRenderer
        {
        private:
            static constexpr size_t mMaxVertices = 2048;

            std::vector<glm::vec3> mPositionBuffer;
            std::vector<glm::vec3> mColorBuffer;

            std::unique_ptr<Shader> mShader;

            GLuint mVertexArray;

            GLuint mPositionVBO;
            GLuint mColorVBO;


            bool mHasBegun = false;
            glm::vec4 mCurrentColor = glm::vec4(1, 1, 1, 1);

            glm::mat4 mProj = glm::mat4(1.f);
            glm::mat4 mView = glm::mat4(1.f);


            void Clear();
            void Flush();

        public:

            ShapeRenderer();
            ~ShapeRenderer();

            void SetColor(const glm::vec4& color);

            void Begin(const glm::mat4& proj, const glm::mat4& view);
            void End();

            void Line(const glm::vec2& p0, const glm::vec2& p1);
            void Circle(const glm::vec2& position, float radius, int segments = 32);
            void Ellipse(const glm::vec2& position, float rX, float rY, int segments = 32);
            void Rect(const glm::vec2& min, const glm::vec2& max);
        };

    }
}
