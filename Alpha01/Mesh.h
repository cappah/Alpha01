#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <assimp\scene.h>

#include <glm\glm.hpp>
#include "ModelShader.h"

namespace gearengine
{
	namespace model
	{
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		struct Texture
		{
			GLuint id;
			std::string type;
			aiString path;
		};

		class Mesh
		{
		public:
			// Mesh data
			std::vector<Vertex> mVertices;
			std::vector<GLuint> mIndices;
			std::vector<Texture> mTextures;
			
			Mesh();
			Mesh(std::vector<Vertex> verts, std::vector<GLuint> ind, std::vector<Texture> texs, graphics::ModelShader& shader);
			~Mesh();

			void draw();

		private:
			graphics::ModelShader mShader;
			GLuint mVAO;
			GLuint mVBO;
			GLuint mEBO;

		private:
			void setupMesh();
		};
	}
}


