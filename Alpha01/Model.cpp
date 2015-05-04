#include "Model.h"
#include "ImageLoad.h"

namespace gearengine
{
	namespace model
	{

		Model::Model()
		{
		}


		Model::Model(graphics::ModelShader& shader, GLchar* path) :
			mShader(shader), mPath(path)
		{
			
		}


		Model::~Model()
		{
		}


		void Model::setModelPlacement(glm::vec3 pos, float scale, float angle)
		{
			setPosition(pos);
			setScale(scale);
			setAngle(angle);
		}

		void Model::setPosition(glm::vec3 pos)
		{
			mPosition = pos;
		}


		void Model::setScale(float scale)
		{
			mScale.x = scale;
			mScale.y = scale;
			mScale.z = scale;
		}


		void Model::setAngle(float angle)
		{
			mAngle = angle;
		}


		void Model::setShaderMatrix(glm::mat4& proj, glm::mat4& view)
		{
			mShader.loadFile(GL_VERTEX_SHADER, "modelvert.glsl");
			mShader.loadFile(GL_FRAGMENT_SHADER, "modelfrag.glsl");
			mShader.linkProgram();
			mShader.enable();

			projectionMatrix = proj;
			viewMatrix = view;

			MVP = projectionMatrix * viewMatrix * modelMatrix;

			scaleMatrix = glm::scale(modelMatrix, mScale);
			rotateMatrix = glm::rotate(modelMatrix, mAngle, glm::vec3(0, 1, 0));

			mShader.setUniformMat4("MVP", MVP);
			mShader.setUniformMat4("scale", scaleMatrix);
			mShader.setUniformMat4("rotate", rotateMatrix);
			mShader.disable();
		}


		void Model::draw()
		{
			mShader.enable();
			
			for (GLuint i = 0; i < meshes.size(); i++)
			{
				meshes[i].draw();				
			}
			
			mShader.disable();
		}


		void Model::init(glm::mat4& proj, glm::mat4& view)
		{
			setShaderMatrix(proj, view);

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(mPath, 
													 aiProcess_Triangulate | 
													 aiProcess_FlipUVs     |
													 aiProcess_GenSmoothNormals);
			// Check for errors
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				return;
			}

			// Retrieve the directory path of the filepath
			this->directory = mPath.substr(0, mPath.find_last_of('/'));

			// Process ASSIMP's root node recursively
			this->processNode(scene->mRootNode, scene);
		}


		void  Model::processNode(aiNode* node, const aiScene* scene)
		{
			for (GLuint i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				this->meshes.push_back(this->processMesh(mesh, scene));
			}
			
			for (GLuint i = 0; i < node->mNumChildren; i++)
			{
				this->processNode(node->mChildren[i], scene);
			}
		}


		Mesh  Model::processMesh(aiMesh* mesh, const aiScene* scene)
		{
			// Data to fill
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Texture> textures;

			// Walk through each of the mesh's vertices
			for (GLuint i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
				// Positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;
				// Normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
				// Texture Coordinates
				if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex);
			}
			// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (GLuint i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// Retrieve all indices of the face and store them in the indices vector
				for (GLuint j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// Process materials
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
				// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
				// Same applies to other texture as the following list summarizes:
				// Diffuse: texture_diffuseN
				// Specular: texture_specularN
				// Normal: texture_normalN

				// 1. Diffuse maps
				std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				// 2. Specular maps
				std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			// Return a mesh object created from the extracted mesh data
			return Mesh(vertices, indices, textures, mShader);
		}


		std::vector<Texture>  Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<Texture> textures;
			for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				GLboolean skip = false;
				for (GLuint j = 0; j < textures_loaded.size(); j++)
				{
					if (textures_loaded[j].path == str)
					{
						std::cout << "Already exists: " << str.C_Str() << std::endl;
						textures.push_back(textures_loaded[j]);
						skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)
				{   // If texture hasn't been loaded already, load it
					Texture texture;
					texture.id = TextureFromFile(str.C_Str(), directory);
					texture.type = typeName;
					texture.path = str;
					textures.push_back(texture);
					textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
					std::cout << "Loading: " << "ID: " << texture.id << ", " << str.C_Str() << std::endl;
				}
			}
			return textures;
		}


		GLuint Model::TextureFromFile(const char* path, std::string directory)
		{
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			GLuint textureID;
			glGenTextures(1, &textureID);
			GLsizei width;
			GLsizei height;

			unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			return textureID;
		}
	}
}