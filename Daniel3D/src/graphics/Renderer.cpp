#include "graphics/Renderer.h"
#include "graphics/Renderable.h"

using namespace dg3d::graphics;

Renderer::Renderer()
{
	glClearColor(0, 0, 0, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer()
{
	index_buffers.clear();
	vertex_arrays.clear();
	shaders.clear();
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::UpdateViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Renderer::Render(const Renderable& renderable) const
{
	if (renderable.render_backface)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
	
	//setup all our needed matrices
	renderable.shader->Bind();
	renderable.shader->SetMat4("u_model", renderable.model);
	renderable.shader->SetMat4("u_view", renderable.view);
	renderable.shader->SetMat4("u_projection", renderable.proj);

	//binds all textures in order they are added to the renderable
	for (uint32_t i = 0; i < renderable.textures.size(); i++)
	{
		renderable.shader->SetInt(renderable.texture_names[i], i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderable.textures[i]->mID);
	}

	glBindVertexArray(renderable.vArray->GetID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.iBuffer->GetID());

	uint32_t instances = renderable.vArray->GetInstanceCount();
	glDrawElementsInstanced(GL_TRIANGLES, renderable.iBuffer->GetElementCount(), GL_UNSIGNED_INT, 0, instances);
}


VertexArray* const Renderer::CreateVertexArray()
{
	auto resource = std::make_unique<VertexArray>();
	auto result = resource.get();

	vertex_arrays.push_back(std::move(resource));

	return result;
}

IndexBuffer* const Renderer::CreateIndexBuffer(const std::vector<unsigned int>& indices)
{
	auto resource = std::make_unique<IndexBuffer>(indices);
	auto result = resource.get();

	index_buffers.push_back(std::move(resource));

	return result;
}

Shader* const Renderer::CreateShader(const char* vertexPath, const char* fragPath)
{
	std::string key = vertexPath;
	key += fragPath;

	auto shaderIter = shaders.find(key);
	if (shaderIter != shaders.end())
	{
		return shaders[key].get();
	}

	shaders[key] = std::make_unique<Shader>(vertexPath, fragPath);
	return shaders[key].get();
}


Renderable* const Renderer::CreateRenderable(Shader* shader, VertexArray* v, IndexBuffer* i)
{
	for (auto& renderable : renderables)
	{
		if (renderable->shader == shader &&
			renderable->vArray == v &&
			renderable->iBuffer == i)
		{
			return renderable.get();
		}
	}

	auto resource = std::make_unique<Renderable>(shader, v, i);
	auto result = resource.get();

	renderables.push_back(std::move(resource));

	return result;
}

Texture* const Renderer::CreateTexture2D(const std::string& filepath)
{
	auto texturesIter = textures.find(filepath);
	if (texturesIter != textures.end())
	{
		return textures[filepath].get();
	}
	
	textures[filepath] = std::unique_ptr<Texture>(Texture::Create(filepath));

	return textures[filepath].get();
}
