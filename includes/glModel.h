#pragma once

#include <vector>

#include "glMesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class GlModel
{
public:
	void draw(GlShader* shader, uint32_t windowID);
	void createVertexArrays();

	GlModel(const std::string& path, GLFWwindow* rendererSharedResources);

private:
	std::vector<std::string> loadedTextureNames;
	std::vector<GlTexture*> loadedTextures;

	std::vector<GlMesh> meshes;
	std::string directory;

	void processNode(aiNode* node, const aiScene* scene);
	GlMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<GlTexture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};