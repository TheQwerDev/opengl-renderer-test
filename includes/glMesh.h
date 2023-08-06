#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>

#include "glVertexArray.h"
#include "glVertexBufferLayout.h"
#include "glVertexBuffer.h"
#include "glIndexBuffer.h"
#include "glShader.h"
#include "glTexture.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class GlMesh {
public:
	void createVertexArray();
	void draw(GlShader* shader, uint32_t windowID);

	GlMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<GlTexture*>& textures);

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<GlTexture*> textures;

private:
	std::vector<GlVertexArray*> vertexArrays;

	GlVertexBuffer* vertexBuffer;
	GlVertexBufferLayout* vertexBufferLayout;

	GlIndexBuffer* indexBuffer;
};