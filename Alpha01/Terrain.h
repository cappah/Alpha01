#pragma once
#include <GL\glew.h>
#include <math.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "maths_func.h"
#include "ModelShader.h"

#include <SOIL.h>

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

namespace gearengine
{
	namespace model
	{
		class Terrain
		{
		public:
			//heightmap texture dimensions and half dimensions
			static const int TERRAIN_WIDTH = 512;
			static const int TERRAIN_DEPTH = 512;
			static const int TERRAIN_HALF_WIDTH = TERRAIN_WIDTH >> 1;
			static const int TERRAIN_HALF_DEPTH = TERRAIN_DEPTH >> 1;

			//total vertices and indices in the terrain
			static const int TOTAL = (TERRAIN_WIDTH * TERRAIN_DEPTH);
			static const int TOTAL_INDICES = TOTAL * 2 * 3;

			
			Terrain();
			Terrain(graphics::ModelShader& s, std::string file);
			~Terrain();
			
			void setShaderMatrix(glm::mat4& proj, glm::mat4& view);
			void init(glm::mat4& proj, glm::mat4& view, glm::vec3& pos);
			void draw();

		private:
			graphics::ModelShader mShader;
			
			//IDs for vertex array and buffer object
			GLuint mVAO;
			GLuint mVBO;
			GLuint mEBO;

			//heighmap texture ID
			GLuint heightMapTextureID;

			//heightmap height scale and half scale values
			float scale = 50;
			float half_scale = scale / 2.0f;

			//height map vertices and indices
			glm::vec3 vertices[TOTAL];
			GLuint indices[TOTAL_INDICES];

			std::string filename;

			glm::mat4 mProjection;
			glm::mat4 mView;
			glm::mat4 mModel = glm::mat4(1.0);
			glm::mat4 MVP;
		};
	}
}


