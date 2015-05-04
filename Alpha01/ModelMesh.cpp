#include "ModelMesh.h"
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gearengine
{
	namespace model
	{
		MMesh::MeshEntry::MeshEntry(const aiScene* scene, aiMesh *mesh) 
		{
			vbo[VERTEX_BUFFER] = NULL;
			vbo[NORMAL_BUFFER] = NULL;
			vbo[TEXCOORD_BUFFER] = NULL;
			vbo[INDEX_BUFFER] = NULL;

			processMesh(scene, mesh);
		}

		/**
		*	Deletes the allocated OpenGL buffers
		**/
		MMesh::MeshEntry::~MeshEntry() 
		{
			if (vbo[VERTEX_BUFFER]) 
			{
				glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
			}

			if (vbo[TEXCOORD_BUFFER]) 
			{
				glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
			}

			if (vbo[NORMAL_BUFFER]) 
			{
				glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
			}

			if (vbo[INDEX_BUFFER]) 
			{
				glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
			}

			glDeleteVertexArrays(1, &vao);
		}


		void MMesh::MeshEntry::processMesh(const aiScene* scene, aiMesh *mesh)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			elementCount = mesh->mNumFaces * 3;

			if (mesh->HasPositions())
			{
				float *vertices = new float[mesh->mNumVertices * 3];
				for (int i = 0; i < mesh->mNumVertices; ++i)
				{
					vertices[i * 3] = mesh->mVertices[i].x;
					vertices[i * 3 + 1] = mesh->mVertices[i].y;
					vertices[i * 3 + 2] = mesh->mVertices[i].z;
				}

				glGenBuffers(1, &vbo[VERTEX_BUFFER]);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
				glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(0);

				delete vertices;
			}


			if (mesh->HasNormals())
			{
				float *normals = new float[mesh->mNumVertices * 3];
				for (int i = 0; i < mesh->mNumVertices; ++i) {
					normals[i * 3] = mesh->mNormals[i].x;
					normals[i * 3 + 1] = mesh->mNormals[i].y;
					normals[i * 3 + 2] = mesh->mNormals[i].z;
				}

				glGenBuffers(1, &vbo[NORMAL_BUFFER]);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
				glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(1);

				delete normals;
			}


			if (mesh->HasTextureCoords(0))
			{
				float *texCoords = new float[mesh->mNumVertices * 2];
				for (int i = 0; i < mesh->mNumVertices; ++i) 
				{
					texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
					texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
				}

				glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
				glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(2);

				delete texCoords;
			}


			if (mesh->HasFaces())
			{
				unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
				for (int i = 0; i < mesh->mNumFaces; ++i) {
					indices[i * 3] = mesh->mFaces[i].mIndices[0];
					indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
					indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
				}

				glGenBuffers(1, &vbo[INDEX_BUFFER]);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(3);

				delete indices;
			}


			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			}



			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}


		/**
		*	Renders this MeshEntry
		**/
		void MMesh::MeshEntry::render() 
		{
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);
		}

		/**
		*	Mesh constructor, loads the specified filename if supported by Assimp
		**/
		MMesh::MMesh(graphics::ModelShader shader, std::string filename) :
			mShader(shader), mFilename(filename)
		{
			
		}

		/**
		*	Clears all loaded MeshEntries
		**/
		MMesh::~MMesh(void)
		{
			for (int i = 0; i < meshEntries.size(); ++i) 
			{
				delete meshEntries.at(i);
			}
			meshEntries.clear();
		}


		void MMesh::init(glm::mat4& proj, glm::mat4& view)
		{
			setShaderMatrix(proj, view);

			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(mFilename, 
				aiProcess_Triangulate |
				aiProcess_FlipUVs |
				aiProcess_GenSmoothNormals);

			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				printf("Unable to load mesh: %s\n", importer.GetErrorString());
			}
			else
				printf("Mesh loaded\n");

			for (int i = 0; i < scene->mNumMeshes; ++i) 
			{
				meshEntries.push_back(new MMesh::MeshEntry(scene, scene->mMeshes[i]));
			}
		}



		/**
		*	Renders all loaded MeshEntries
		**/
		void MMesh::render() {
			
			/*GLuint diffuseNr = 1;
			GLuint specularNr = 1;

			for (GLuint i = 0; i < mTextures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				std::stringstream ss;
				std::string number;
				std::string name = mTextures[i].type;

				if (name == "texture_diffuse")
					ss << diffuseNr++;
				else if (name == "texture_specular")
					ss << specularNr++;

				number = ss.str();

				mShader.setUniform1i((name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
			}

			mShader.setUniform1i("material.shininess", 16.0f);*/

			mShader.enable();
			for (int i = 0; i < meshEntries.size(); ++i) {
				meshEntries.at(i)->render();
			}
			mShader.disable();
		}


		void MMesh::setShaderMatrix(glm::mat4& proj, glm::mat4& view)
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


		void MMesh::setModelPlacement(glm::vec3 pos, float scale, float angle)
		{
			setPosition(pos);
			setScale(scale);
			setAngle(angle);
		}

		void MMesh::setPosition(glm::vec3 pos)
		{
			mPosition = pos;
		}


		void MMesh::setScale(float scale)
		{
			mScale.x = scale;
			mScale.y = scale;
			mScale.z = scale;
		}


		void MMesh::setAngle(float angle)
		{
			mAngle = angle;
		}
	}
}