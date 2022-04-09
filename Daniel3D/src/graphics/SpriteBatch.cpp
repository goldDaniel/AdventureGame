#include <graphics/SpriteBatch.h>

#include <graphics/Shader.h>
#include <graphics/TextureRegion.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace dg3d::graphics;

static const std::string sVertexCode =
"#version 330 core\n"

"layout(location = 0) in vec3 a_position;\n"
"layout(location = 1) in vec3 a_color;\n"
"layout(location = 2) in vec2 a_texcoord;\n"

"uniform mat4 u_proj;\n"
"uniform mat4 u_view;\n"

"out vec3 Color;\n"
"out vec2 Texcoord;\n"

"void main()\n"
"{"
"    Color = a_color;\n"
"    Texcoord = vec2(0, 1) + vec2(a_texcoord.x, -a_texcoord.y);\n"
"    gl_Position = u_proj * u_view * vec4(a_position.xyz, 1.0);\n"
"}";

static const std::string sFragmentCode =
"in vec3 Color;\n"
"in vec2 Texcoord;\n"

"uniform sampler2D u_texture;\n"

"void main()\n"
"{"
"   vec4 texture_color = texture(u_texture, Texcoord);\n"
"   gl_FragColor = vec4(texture_color.rgb * Color, texture_color.a);\n"
"}";

SpriteBatch::SpriteBatch(const size_t maxSprites)
	: mMaxSprites(maxSprites)
{
    mPositionBuffer.reserve(mMaxSprites * 4);
    mColorBuffer.reserve(mMaxSprites * 4);
	mTexCoordBuffer.reserve(mMaxSprites * 4);
    mIndicesBuffer.reserve(mMaxSprites * 6);
     
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mPositionVBO);
	glGenBuffers(1, &mColorVBO);
	glGenBuffers(1, &mTexCoordVBO);
	glGenBuffers(1, &mEBO);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    shader = std::make_unique<Shader>(sVertexCode, sFragmentCode, true);
}

SpriteBatch::~SpriteBatch()
{
	glDeleteBuffers(1, &mEBO);
	glDeleteBuffers(1, &mTexCoordVBO);
	glDeleteBuffers(1, &mColorVBO);
	glDeleteBuffers(1, &mPositionVBO);

	glDeleteVertexArrays(1, &mVertexArray);
}

void SpriteBatch::SetColor(const glm::vec4& color)
{
    mCurrentColor = { color.r, color.g, color.b };
}

void SpriteBatch::Begin(const glm::mat4& proj, const glm::mat4& view)
{
	assert(!mIsRunning && "SpriteBatch::End() must be called before SpriteBatch::Begin()");

	mProj = proj;
	mView = view;

	mIsRunning = true;
	Clear();
}

void SpriteBatch::End()
{
	assert(mIsRunning && "SpriteBatch::Begin() must be called before SpriteBatch::End()");

	Flush();
	Clear();

	mIsRunning = false;
}

void SpriteBatch::Draw(TextureRegion const* tex, float x, float y, float w, float h)
{

    Draw(tex, x, y, w, h, 0);
}

void SpriteBatch::Draw(TextureRegion const* tex, float x, float y, float w, float h, float angle)
{
    glm::vec2 size{ tex->GetTexture()->mWidth, tex->GetTexture()->mHeight };
    glm::vec4 region = tex->GetRegion();

    glm::vec2 uvMin = { region.x / size.x, region.y / size.y };
    glm::vec2 uvMax = { region.z / size.x, region.w / size.y };

    Draw(tex->GetTexture(), x, y, w, h, 0, uvMin, uvMax);
}

void SpriteBatch::Draw(Texture const* tex, float x, float y, float width, float height)
{
    Draw(tex, x, y, width, height, 0);
}

void SpriteBatch::Draw(Texture const * tex, float x, float y, float w, float h, float angle)
{
    Draw(tex, x, y, w, h, 0, { 0,0 }, { 1, 1 });
}

void SpriteBatch::Draw(Texture const* tex, float x, float y, float w, float h, float angle, glm::vec2 uvMin, glm::vec2 uvMax)
{
    if (tex != mCurrentTexture)
    {
        Flush();
        Clear();    
    }
    if (mPositionBuffer.size() / 4 >= mMaxSprites)
    {
        Flush();
        Clear();
    }
    mCurrentTexture = tex;

    glm::mat4 rot = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.0f, 0.0f, -1.0f));

    glm::vec4 p0(-w / 2.f, -h / 2.f, 0, 1);
    glm::vec4 p1(w / 2.f, -h / 2.f, 0, 1);
    glm::vec4 p2(w / 2.f, h / 2.f, 0, 1);
    glm::vec4 p3(-w / 2.f, h / 2.f, 0, 1);

    p0 = p0 * rot;
    p1 = p1 * rot;
    p2 = p2 * rot;
    p3 = p3 * rot;

    mPositionBuffer.push_back(glm::vec3{ x + p0.x, y + p0.y, 0 });
    mPositionBuffer.push_back(glm::vec3{ x + p1.x, y + p1.y, 0 });
    mPositionBuffer.push_back(glm::vec3{ x + p2.x, y + p2.y, 0 });
    mPositionBuffer.push_back(glm::vec3{ x + p3.x, y + p3.y, 0 });

    for (int i = 0; i < 4; i++)
    {
        mColorBuffer.push_back(glm::vec3{ mCurrentColor.x, mCurrentColor.y, mCurrentColor.z });
    }

    mTexCoordBuffer.push_back(uvMin);
    mTexCoordBuffer.push_back(glm::vec2{ uvMax.x, uvMin.y });
    mTexCoordBuffer.push_back(uvMax);
    mTexCoordBuffer.push_back(glm::vec2{ uvMin.x, uvMax.y });

    size_t currentVertexIndex = mPositionBuffer.size() - 4;
    mIndicesBuffer.push_back(currentVertexIndex + 0);
    mIndicesBuffer.push_back(currentVertexIndex + 1);
    mIndicesBuffer.push_back(currentVertexIndex + 3);

    mIndicesBuffer.push_back(currentVertexIndex + 3);
    mIndicesBuffer.push_back(currentVertexIndex + 1);
    mIndicesBuffer.push_back(currentVertexIndex + 2);
}

void SpriteBatch::Clear()
{
    mPositionBuffer.clear();
    mColorBuffer.clear();
    mTexCoordBuffer.clear();
    mIndicesBuffer.clear();
   
    mCurrentTexture = nullptr;
}

void SpriteBatch::Flush()
{
    if (mCurrentTexture == nullptr) return;

    assert(mIsRunning && "ShapeRenderer must be running to flush data to GPU");

    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mPositionBuffer.size(), &mPositionBuffer[0], GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mColorBuffer.size(), &mColorBuffer[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mTexCoordBuffer.size(), &mTexCoordBuffer[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndicesBuffer.size(), &mIndicesBuffer[0], GL_DYNAMIC_DRAW);

    shader->Bind();
    shader->SetMat4("u_proj", mProj);
    shader->SetMat4("u_view", mView);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mCurrentTexture->mID);
    shader->SetInt("u_texture", 0);

    glDrawElements(GL_TRIANGLES, mIndicesBuffer.size(), GL_UNSIGNED_INT, 0);
}