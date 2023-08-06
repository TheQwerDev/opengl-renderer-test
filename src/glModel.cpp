#include <iostream>
#include <fstream>

#include "glModel.h"

void GlModel::draw(GlShader* shader, uint32_t windowID)
{
	for (uint32_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader, windowID);
	}
}

void GlModel::createVertexArrays()
{
	for (uint32_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].createVertexArray();
	}
}

GlModel::GlModel(const std::string& path, GLFWwindow* rendererSharedResources)
{
	glfwMakeContextCurrent(rendererSharedResources);

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::ofstream log("log.txt", std::ios_base::app);

		const char* err = importer.GetErrorString();

		std::cout << "[Assimp] ERROR: " << err << '\n';
		log << "[Assimp] ERROR: " << err << '\n';

		log.close();

		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	glfwMakeContextCurrent(NULL);
}

void GlModel::processNode(aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

GlMesh GlModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<GlTexture*> textures;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textureCoords;
			textureCoords.x = mesh->mTextureCoords[0][i].x;
			textureCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = textureCoords;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<GlTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<GlTexture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());	
	}

	return GlMesh(vertices, indices, textures);
}

std::vector<GlTexture*> GlModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<GlTexture*> textures;

	uint32_t textureCount = mat->GetTextureCount(type);

	if (textureCount == 0)
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		GlTexture* diffuse = new GlTexture(color.r, color.g, color.b, DIFFUSE);

		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		GlTexture* specular = new GlTexture(color.r, color.g, color.b, SPECULAR);

		//could check if the colors obtained aren't already used for another mesh,
		//but that's currently not a priority due to the small amount of memory used per
		//textureless mesh (6 bytes)
		
		textures.push_back(diffuse);
		textures.push_back(specular);
	}
	else for (uint32_t i = 0; i < textureCount; i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string textureName = str.data;

		bool skip = false;
		for (uint32_t j = 0; j < loadedTextureNames.size(); j++)
		{
 			if (std::strcmp(textureName.c_str(), loadedTextureNames[j].c_str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		TextureType convertedType = NONE;
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			convertedType = DIFFUSE;
			break;

		case aiTextureType_SPECULAR:
			convertedType = SPECULAR;
			break;
		}

		if (!skip)
		{
			GlTexture* texture = new GlTexture((directory + '/' + textureName).c_str(), GL_REPEAT, GL_LINEAR, convertedType);

 			loadedTextureNames.push_back(textureName);
			loadedTextures.push_back(texture);

			textures.push_back(texture);
		}
	}

	return textures;
}
