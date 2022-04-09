#include <graphics/IndexBuffer.h>

using namespace dg3d::graphics;

IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices)
{
	glGenBuffers(1, &mID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	mElementCount = static_cast<GLuint>(indices.size());
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
}

uint32_t IndexBuffer::GetID() const
{
	return mID;
}

uint32_t IndexBuffer::GetElementCount() const
{
	return mElementCount;
}