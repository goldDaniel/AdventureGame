#include "graphics/VertexArray.h"

using namespace dg3d::graphics;

VertexArray::VertexArray() 
	: mInstanceCount(1)
{
	glGenVertexArrays(1, &mID);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(mVertexBuffers.size(), &mVertexBuffers[0]);
	glDeleteVertexArrays(1, &mID);
}

void VertexArray::SetNumInstances(uint32_t instances)
{
	mInstanceCount = instances;
}

uint32_t VertexArray::GetInstanceCount() const
{
	return mInstanceCount;
}

void VertexArray::AddVertexBuffer(const std::vector<float>& vertexBuffer, uint32_t elementsPerVertex)
{
	GLuint buffer;

	glBindVertexArray(mID);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexBuffer.size(), &vertexBuffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(mVertexBufferIndex);
	glVertexAttribPointer(mVertexBufferIndex, elementsPerVertex, GL_FLOAT, false, elementsPerVertex * sizeof(float), (void*)0);

	mVertexBufferIndex++;
	mVertexBuffers.push_back(buffer);
}

void VertexArray::AddInstancedVertexBuffer(const std::vector<glm::mat4>& matrices)
{
	glBindVertexArray(mID);

	uint32_t buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(mVertexBufferIndex);
	glVertexAttribPointer(mVertexBufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribDivisor(mVertexBufferIndex, 1);
	mVertexBufferIndex++;

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(mVertexBufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribDivisor(mVertexBufferIndex, 1);
	mVertexBufferIndex++;

	glEnableVertexAttribArray(mVertexBufferIndex);
	glVertexAttribPointer(mVertexBufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribDivisor(mVertexBufferIndex, 1);
	mVertexBufferIndex++;

	glEnableVertexAttribArray(mVertexBufferIndex);
	glVertexAttribPointer(mVertexBufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(mVertexBufferIndex, 1);
	mVertexBufferIndex++;

	mVertexBuffers.push_back(buffer);
}

void VertexArray::AddInstancedVertexBuffer(std::vector<float>& vertexBuffer, uint32_t elementsPerVertex, uint32_t attribDivisor)
{
	GLuint buffer;

	glBindVertexArray(mID);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexBuffer.size(), &vertexBuffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(mVertexBufferIndex);
	glVertexAttribPointer(mVertexBufferIndex, elementsPerVertex, GL_FLOAT, false, elementsPerVertex * sizeof(float), (void*)0);
	glVertexAttribDivisor(mVertexBufferIndex, attribDivisor);

	mVertexBufferIndex++;
	mVertexBuffers.push_back(buffer);
}

void VertexArray::Bind() const
{
	glBindVertexArray(mID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

uint32_t VertexArray::GetID() const
{
	return mID;
}
