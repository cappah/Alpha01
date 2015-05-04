#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <iostream>
#include "ModelShader.h"
#include "Mesh.h"
#include <SOIL.h>

namespace gearengine
{
	namespace model
	{


		class Model
		{
		public:
			Model();
			Model(graphics::ModelShader& shader, GLchar* path);
			~Model();

			void init(glm::mat4& proj, glm::mat4& view);
			void setModelPlacement(glm::vec3 pos, float scale, float angle);
			void setPosition(glm::vec3 pos);
			void setScale(float scale);
			void setAngle(float angle);
			void setShaderMatrix(glm::mat4& proj, glm::mat4& view);
			void draw();
			
			void enable() { mShader.enable(); }
			void disable() { mShader.disable(); }

			inline glm::vec3 getPosition() const { return mPosition; }
			inline float getAngle() { return mAngle; }
			void addToAngle(float amount) { mAngle += amount; }
		private:
			std::string directory;
			std::vector<Mesh> meshes;
			std::vector<Texture> textures_loaded;

			glm::vec3 mPosition;
			glm::vec3 mScale;
			float mAngle;

			glm::mat4 MVP;
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 modelMatrix;
			glm::mat4 scaleMatrix;
			glm::mat4 rotateMatrix;

			graphics::ModelShader mShader;
			const std::string mPath;

		private:
			
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType, std::string typeName);
			GLuint TextureFromFile(const char* path, std::string directory);
		};



	}
}


