#pragma once
#include <GL\glew.h>
#include <math.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "Commons.h"
#include "maths_func.h"
#include "ModelShader.h"
#include "TTexLoader.h"
#include <FreeImage.h>
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
			static const int TERRAIN_SIZE = 1024;
			static const int TERRAIN_HALF_SIZE = TERRAIN_SIZE >> 1;
			

			//total vertices and indices in the terrain
			static const int TOTAL = (TERRAIN_SIZE * TERRAIN_SIZE);
			static const int TOTAL_INDICES = TOTAL * 2 * 3;

			
			Terrain();
			Terrain(graphics::ModelShader& s, std::string file, std::string texFile);
			~Terrain();
			
			void setShaderMatrix(glm::mat4& proj, glm::mat4& view);
			void init(glm::mat4& proj, glm::mat4& view, glm::vec3& pos);
			void generateVertices();
			void generateIndices();
			void generateTexCoords();
			void generateNormals();
			void draw();

			inline glm::vec3 getVertPos(int pos) const { return vertices[pos]; }
		private:
			
			graphics::ModelShader mShader;
			
			//IDs for vertex array and buffer object
			GLuint mVAO;
			GLuint mVBO;
			GLuint mEBO;

			//heighmap texture ID
			GLuint heightMapTextureID;
			GLuint textureID;
			GLuint sampler;

			//heightmap height scale and half scale values
			float scale = 50;
			float half_scale = scale / 2.0f;

			int count;
			int iRows;
			int iCols;

			//height map vertices and indices
			glm::vec3 vertices[TOTAL];
			std::vector< std::vector< glm::vec3>> vertexDataArray;
			std::vector< std::vector< glm::vec2> > coordsDataArray;

			GLuint indices[TOTAL_INDICES];
			glm::vec2 texCoords[TOTAL];

			std::string filename;
			std::string texFilename;

			glm::mat4 mProjection;
			glm::mat4 mView;
			glm::mat4 mModel = glm::mat4(1.0);
			glm::mat4 MVP;

			std::vector<BYTE> mData;
			int mCurrentSize;
			int mSize;
			int mBufferType;

			graphics::TTexLoader texture;

		private:
			void loadHeightMap();
			void storePositions(GLubyte* pointer, unsigned int rowStep, unsigned int ptr);

			
			
		};
	}
}


