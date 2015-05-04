#include "Terrain.h"
#include "maths_func.h"
#include <iostream>
#include <algorithm>
#include <cassert>
//#include <cstdlib>
//#include <ctime>

#define BUFFER_OFFSET(offset) ((void *)(offset)) 

namespace gearengine
{
	namespace model
	{
		Terrain::Terrain()
		{

		}


		Terrain::Terrain(graphics::ModelShader& shader, std::string file) :
			mShader(shader), filename(file)
		{

		}


		Terrain::~Terrain()
		{
			//Destroy shader
			mShader.DeleteShaderProgram();

			//Destroy vao and vbo
			glDeleteBuffers(1, &mVBO);
			glDeleteBuffers(1, &mEBO);
			glDeleteVertexArrays(1, &mVAO);

			//Delete textures
			glDeleteTextures(1, &heightMapTextureID);
			std::cout << "Shutdown successfull" << std::endl;
		}


		void Terrain::init(glm::mat4& proj, glm::mat4& view, glm::vec3& pos)
		{
			
			//load heightmap shader
			
			mShader.loadFile(GL_VERTEX_SHADER, "terrainvert.glsl");
			mShader.loadFile(GL_FRAGMENT_SHADER, "terrainfrag.glsl");

			//compile and link shader
			mShader.linkProgram();
			mShader.enable();

			//add attributes and uniforms
			mShader.addAttribute("position");
			mShader.addUniform("heightMapTexture");
			mShader.addUniform("scale");
			mShader.addUniform("half_scale");
			mShader.addUniform("HALF_TERRAIN_SIZE");
			mShader.addUniform("MVP");

			//set values of constant uniforms as initialization	
			mShader.setUniform1i("heightMapTexture", 0);
			mShader.setUniform2i("HALF_TERRAIN_SIZE", TERRAIN_WIDTH >> 1, TERRAIN_DEPTH >> 1);
			mShader.setUniform1f("scale", scale);
			mShader.setUniform1f("half_scale", half_scale);

			mProjection = proj;
			mView = view;		
			glm::mat4 trans = glm::translate(mModel, pos);
			MVP = mProjection * mView * mModel * trans;

			//pass mShader uniforms
			mShader.setUniformMat4("MVP", MVP);
			mShader.disable();

			//fill indices array
			GLuint* ind = &indices[0];
			int i = 0, j = 0;

			//setup vertices 
			int count = 0;
			//fill terrain vertices
			for (j = 0; j<TERRAIN_DEPTH; j++) 
			{
				for (i = 0; i<TERRAIN_WIDTH; i++) 
				{
					vertices[count] = glm::vec3((float(i) / (TERRAIN_WIDTH - 1)),
						0,
						(float(j) / (TERRAIN_DEPTH - 1)));
					count++;
				}
			}

			//fill terrain indices
			for (i = 0; i < TERRAIN_DEPTH - 1; i++) 
			{
				for (j = 0; j < TERRAIN_WIDTH - 1; j++) 
				{
					int i0 = j + i * TERRAIN_WIDTH;
					int i1 = i0 + 1;
					int i2 = i0 + TERRAIN_WIDTH;
					int i3 = i2 + 1;
					*ind++ = i0;
					*ind++ = i2;
					*ind++ = i1;
					*ind++ = i1;
					*ind++ = i2;
					*ind++ = i3;
				}
			}


			//setup terrain vertex array and vertex buffer objects 
			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mVBO);
			glGenBuffers(1, &mEBO);

			glBindVertexArray(mVAO);

			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			//pass terrain vertices to buffer object
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	
			//enable vertex attribute array for position
			glEnableVertexAttribArray(mShader["position"]);
			glVertexAttribPointer(mShader["position"], 3, GL_FLOAT, GL_FALSE, 0, 0);
			
			//pass the terrain indices array to element array buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
			

			//load the heightmap texture using SOIL	
			int texture_width = 0, texture_height = 0, channels = 0;
			GLubyte* pData = SOIL_load_image(filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_L);

			//vertically flip the heightmap image on Y axis since it is inverted 
			for (j = 0; j * 2 < texture_height; ++j)
			{
				int index1 = j * texture_width;
				int index2 = (texture_height - 1 - j) * texture_width;
				for (i = texture_width; i > 0; --i)
				{
					GLubyte temp = pData[index1];
					pData[index1] = pData[index2];
					pData[index2] = temp;
					++index1;
					++index2;
				}
			}

			//setup OpenGL texture
			glGenTextures(1, &heightMapTextureID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMapTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_width, texture_height, 0, GL_RED, GL_UNSIGNED_BYTE, pData);

			//free SOIL image data
			SOIL_free_image_data(pData);

			//set polygon mode to draw lines
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		void Terrain::draw()
		{
			//bind the terrain mShader
			mShader.enable();
			glBindVertexArray(mVAO);	
			//draw terrain mesh
			glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			//unbind mShader
			mShader.disable();
		}
	}
}