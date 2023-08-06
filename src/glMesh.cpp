#include "glad/glad.h"

#include "glMesh.h"

void GlMesh::createVertexArray()
{
	GlVertexArray* vertexArray = new GlVertexArray();
	vertexArray->addBuffer(*vertexBuffer, *vertexBufferLayout);

	vertexArrays.push_back(vertexArray);
}

void GlMesh::draw(GlShader* shader, uint32_t windowID)
{
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;

	for (uint32_t i = 0; i < textures.size(); i++)
	{
		textures[i]->bind(i);

		std::string name;
		TextureType type = textures[i]->getType();

		switch (type)
		{
		case DIFFUSE:
			name = "texture_diffuse" + std::to_string(diffuseNr++);
			break;

		case SPECULAR:
			name = "texture_specular" + std::to_string(specularNr++);
			break;
		}

		shader->setInt(("material." + name).c_str(), i);
	}

	vertexArrays[windowID]->bind();
	indexBuffer->bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	vertexArrays[windowID]->unbind();

	indexBuffer->unbind();
}

GlMesh::GlMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<GlTexture*>& textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	vertexBuffer = new GlVertexBuffer(vertices.data(), (uint32_t)(vertices.size() * sizeof(Vertex)));
	indexBuffer = new GlIndexBuffer(indices.data(), (uint32_t)indices.size());

	vertexBufferLayout = new GlVertexBufferLayout();
	vertexBufferLayout->push<float>(3); //vertex positions
	vertexBufferLayout->push<float>(3); //vertex normals
	vertexBufferLayout->push<float>(2); //vertex texture coords
}