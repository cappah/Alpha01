#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>

#include "ModelShader.h"

namespace gearengine
{
	namespace model
	{
		class MMesh
		{
		public:
			struct MeshEntry {
				static enum BUFFERS {
					VERTEX_BUFFER, NORMAL_BUFFER, TEXCOORD_BUFFER, INDEX_BUFFER
				};
				GLuint vao;
				GLuint vbo[4];

				unsigned int elementCount;

				MeshEntry(const aiScene* scene, aiMesh *mesh);
				~MeshEntry();

				void processMesh(const aiScene* scene, aiMesh *mesh);
				
				void load(aiMesh *mesh);
				void render();
			};

			std::vector<MeshEntry*> meshEntries;

		public:
			MMesh(graphics::ModelShader shader, std::string filename);
			~MMesh(void);
			void init(glm::mat4& proj, glm::mat4& view);
			void render();

			void setModelPlacement(glm::vec3 pos, float scale, float angle);
			void setPosition(glm::vec3 pos);
			void setScale(float scale);
			void setAngle(float angle);

		private:
			void setShaderMatrix(glm::mat4& proj, glm::mat4& view);

			graphics::ModelShader mShader;
			
			std::string mFilename;
			std::string mDirectory;

			glm::vec3 mPosition;
			glm::vec3 mScale;
			float mAngle;

			glm::mat4 MVP;
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 modelMatrix;
			glm::mat4 scaleMatrix;
			glm::mat4 rotateMatrix;
		};
	}
}

