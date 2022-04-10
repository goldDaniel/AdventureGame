#include <graphics/ShapeRenderer.h>

#include <cassert>

#include <glm/gtc/constants.hpp>

using namespace dg3d::graphics;

static const std::string vertexcode =
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 a_position;\n"
"layout(location = 1) in vec3 a_color;\n"
"\n"
"uniform mat4 u_proj;\n"
"uniform mat4 u_view;\n"
"\n"
"out vec3 Color;\n"
"\n"
"void main()\n"
"{\n"
"    Color = a_color;\n"
"    gl_Position = u_proj * u_view * vec4(a_position.xyz, 1.0);\n"
"}\n";

static const std::string fragmentcode =
"#version 330 core\n"
"\n"
"in vec3 Color;\n"
"\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(Color, 1.0);\n"
"}\n";

ShapeRenderer::ShapeRenderer()
{
    mShader = std::make_unique<Shader>(vertexcode, fragmentcode, true);

    glGenVertexArrays(1, &mVertexArray);

    glGenBuffers(1, &mPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &mColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

ShapeRenderer::~ShapeRenderer()
{
    glDeleteBuffers(1, &mColorVBO);
    glDeleteBuffers(1, &mPositionVBO);

    glDeleteVertexArrays(1, &mVertexArray);
}

void ShapeRenderer::Clear()
{
    mPositionBuffer.clear();
    mColorBuffer.clear();
}

void ShapeRenderer::Flush()
{
    assert(mHasBegun && "ShapeRenderer must be running to flush data to GPU");
    if (mPositionBuffer.size() == 0) return;
    
    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mPositionBuffer.size(), &mPositionBuffer[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mColorBuffer.size(), &mColorBuffer[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    mShader->Bind();
    mShader->SetMat4("u_proj", mProj);
    mShader->SetMat4("u_view", mView);

    glDrawArrays(GL_LINES, 0, mPositionBuffer.size());
}

void ShapeRenderer::SetColor(const glm::vec4& color)
{
    mCurrentColor = color;
}

void ShapeRenderer::Begin(const glm::mat4& proj, const glm::mat4& view)
{
    assert(!mHasBegun && "ShapeRenderer->End() must be called before ShapeRenderer->Begin()");

    Clear();
    mProj = proj;
    mView = view;

    mHasBegun = true;
}

void ShapeRenderer::End()
{
    assert(mHasBegun && "ShapeRenderer->Begin() must be called before ShapeRenderer->End()");

    Flush();
    Clear();

    mHasBegun = false;
}

void ShapeRenderer::Line(const glm::vec2& p0, const glm::vec2& p1)
{
    if (mPositionBuffer.size() >= mMaxVertices - 2)
    {
        Flush();
        Clear();
    }

    mPositionBuffer.push_back({p0, 0});
    mColorBuffer.push_back(mCurrentColor);

    mPositionBuffer.push_back({ p1, 0 });
    mColorBuffer.push_back(mCurrentColor);
}

void ShapeRenderer::Circle(const glm::vec2& position, float radius, int segments)
{
    Ellipse(position, radius, radius, segments);
}

void ShapeRenderer::Ellipse(const glm::vec2& position, float rX, float rY, int segments)
{
    for (int i = 0; i < segments; i++)
    {
        float angle0 = (float)i / (float)segments * glm::pi<float>() * 2;
        float angle1 = (float)(i + 1) / (float)segments * glm::pi<float>() * 2;

        glm::vec2 p0 = position + glm::vec2(glm::cos(angle0) * rX, glm::sin(angle0) * rY);
        glm::vec2 p1 = position + glm::vec2(glm::cos(angle1) * rX, glm::sin(angle1) * rY);

        Line(p0, p1);
    }
}

void ShapeRenderer::Rect(const glm::vec2& min, const glm::vec2& max)
{
    glm::vec2 p0(min.x, min.y);
    glm::vec2 p1(max.x, min.y);
    glm::vec2 p2(max.x, max.y);
    glm::vec2 p3(min.x, max.y);

    Line(p0, p1);
    Line(p1, p2);
    Line(p2, p3);
    Line(p3, p0);
}