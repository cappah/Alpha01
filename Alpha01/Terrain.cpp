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


		Terrain::Terrain(graphics::ModelShader& shader, std::string file, std::string texFile) :
			mShader(shader), filename(file), texFilename(texFile)
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
			std::cout << "Shutdown successful" << std::endl;
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
			mShader.setUniform2i("HALF_TERRAIN_SIZE", TERRAIN_SIZE >> 1, TERRAIN_SIZE >> 1);
			mShader.setUniform1f("scale", scale);
			mShader.setUniform1f("half_scale", half_scale);
			mShader.setUniform1i("tex", 1);
			//mShader.setUniform4f("vColor", glm::vec4(1, 1, 1, 1));

			mProjection = proj;
			mView = view;		
			glm::mat4 trans = glm::translate(mModel, pos);
			MVP = mProjection * mView * mModel * trans;

			//pass mShader uniforms
			mShader.setUniformMat4("MVP", MVP);
			mShader.disable();

			loadHeightMap();			
		}


		void Terrain::loadHeightMap()
		{
			//setup terrain vertex array and vertex buffer objects 
			glGenVertexArrays(1, &mVAO);
			glBindVertexArray(mVAO);
			
			generateVertices();
			generateIndices();
			//generateTexCoords();
			

			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			FIBITMAP* dib(0);

			fif = FreeImage_GetFileType(filename.c_str(), 0); // Check the file signature and deduce its format

			if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
				fif = FreeImage_GetFIFFromFilename(filename.c_str());

			if (fif == FIF_UNKNOWN) // If still unknown, return failure
				std::cout << "UNKNOWN" << std::endl;

			if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
				dib = FreeImage_Load(fif, filename.c_str());
			if (!dib)
				std::cout << "!dib" << std::endl;

			GLubyte* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
			iRows = FreeImage_GetHeight(dib);
			iCols = FreeImage_GetWidth(dib);

			// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
			if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
				std::cout << "BAD" << std::endl;

			// How much to increase data pointer to get to next pixel data
			unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
			// Length of one row in data
			unsigned int row_step = ptr_inc*iCols;

			//vertically flip the heightmap image on Y axis since it is inverted 
			for (int j = 0; j * 2 < iRows; ++j)
			{
				int index1 = j * iCols;
				int index2 = (iRows - 1 - j) * iCols;
				for (int i = iCols; i > 0; --i)
				{
					GLubyte temp = bDataPointer[index1];
					bDataPointer[index1] = bDataPointer[index2];
					bDataPointer[index2] = temp;
					++index1;
					++index2;
				}
			}

			// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
			storePositions(bDataPointer, row_step, ptr_inc);

			//setup OpenGL heightmap texture
			glGenTextures(1, &heightMapTextureID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMapTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, iCols, iRows, 0, GL_RED, GL_UNSIGNED_BYTE, bDataPointer);
			std::cout << "Heightmap ID: " << heightMapTextureID << std::endl;
			
			FreeImage_Unload(dib);

			

			//set polygon mode to draw lines
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		void Terrain::storePositions(GLubyte* pointer, unsigned int rowStep, unsigned int ptr)
		{
			// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
			std::vector< std::vector< glm::vec3>> vVertexData(iRows, std::vector<glm::vec3>(iCols));
			std::vector< std::vector< glm::vec2> > vCoordsData(iRows, std::vector<glm::vec2>(iCols));

			float fTextureU = float(iCols)*0.1f;
			float fTextureV = float(iRows)*0.1f;

			FOR(i, iRows)
			{
				FOR(j, iCols)
				{
					float fScaleC = float(j) / float(iCols - 1);
					float fScaleR = float(i) / float(iRows - 1);
					float fVertexHeight = float(*(pointer + rowStep * i + j * ptr)) / 255.0f;
					vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
					vCoordsData[i][j] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
				}
			}
			vertexDataArray = vVertexData;
			coordsDataArray = vCoordsData;

			generateNormals();
		}

		void Terrain::generateVertices()
		{
			count = 0;
			for (int j = 0; j < TERRAIN_SIZE; j++)
			{
				for (int i = 0; i < TERRAIN_SIZE; i++)
				{
					vertices[count] = glm::vec3(
						(float(i) / (TERRAIN_SIZE - 1)),
						0,
						(float(j) / (TERRAIN_SIZE - 1)));

					count++;
				}
			}

			glGenBuffers(1, &mVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(mShader["position"]);
			glVertexAttribPointer(mShader["position"], 3, GL_FLOAT, GL_FALSE, 0, 0);
			count = 0;
		}


		void Terrain::generateIndices()
		{
			GLuint* ind = &indices[0];
			for (int i = 0; i < TERRAIN_SIZE - 1; i++)
			{
				for (int j = 0; j < TERRAIN_SIZE - 1; j++)
				{
					int i0 = j + i * TERRAIN_SIZE;
					int i1 = i0 + 1;
					int i2 = i0 + TERRAIN_SIZE;
					int i3 = i2 + 1;
					*ind++ = i0;
					*ind++ = i2;
					*ind++ = i1;
					*ind++ = i1;
					*ind++ = i2;
					*ind++ = i3;
				}
			}
			//pass the terrain indices array to element array buffer
			glGenBuffers(1, &mEBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		}


		void Terrain::generateTexCoords()
		{
			for (int z = 0; z < TERRAIN_SIZE; z++)
			{
				for (int x = 0; x < TERRAIN_SIZE; x++)
				{
					float s = (float(x) / float(TERRAIN_SIZE)) * 8.0f;
					float t = (float(z) / float(TERRAIN_SIZE)) * 8.0f;
					texCoords[count] = glm::vec2(s, t);
					count++;
				}
			}
			
			
			//if (texture.LoadTexture2D("textures/ground/natural/sand_grass_02.jpg", true))
			//	std::cout << "READ TERRAIN TEXTURE" << std::endl;


			//glGenBuffers(1, &textureID);
			//glBindBuffer(GL_ARRAY_BUFFER, textureID); //Bind the vertex buffer
			//glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), &texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
			
		}


		void Terrain::generateNormals()
		{
			std::vector<std::vector<glm::vec3>> normals[2];

			FOR(i, 2) normals[i] = std::vector<std::vector<glm::vec3>>(iRows - 1, std::vector<glm::vec3>(iCols - 1));

			FOR(i, iRows - 1)
			{
				FOR(j, iCols - 1)
				{
					glm::vec3 triangle0[] =
					{
						vertexDataArray[i][j],
						vertexDataArray[i + 1][j],
						vertexDataArray[i + 1][j + 1],
					};

					glm::vec3 triangle1[] =
					{
						vertexDataArray[i + 1][j + 1],
						vertexDataArray[i][j + 1],
						vertexDataArray[i][j],
					};

					glm::vec3 triangleNorm0 = glm::cross(triangle0[0] - triangle0[1], triangle0[1] - triangle0[2]);
					glm::vec3 triangleNorm1 = glm::cross(triangle1[0] - triangle1[1], triangle1[1] - triangle1[2]);

					normals[0][i][j] = glm::normalize(triangleNorm0);
					normals[1][i][j] = glm::normalize(triangleNorm1);
				}
			}

			std::vector<std::vector<glm::vec3>> vFinalNormals = std::vector<std::vector<glm::vec3> >(iRows, std::vector<glm::vec3>(iCols));

			FOR(i, iRows)
			{
				FOR(j, iCols)
				{
					// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
					// of all adjacent triangles' normals

					glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

					// Look for upper-left triangles
					if (j != 0 && i != 0)
						FOR(k, 2)vFinalNormal += normals[k][i - 1][j - 1];
					// Look for upper-right triangles
					if (i != 0 && j != iCols - 1)vFinalNormal += normals[0][i - 1][j];
					// Look for bottom-right triangles
					if (i != iRows - 1 && j != iCols - 1)
						FOR(k, 2)vFinalNormal += normals[k][i][j];
					// Look for bottom-left triangles
					if (i != iRows - 1 && j != 0)
						vFinalNormal += normals[1][i][j - 1];
					vFinalNormal = glm::normalize(vFinalNormal);

					vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
				}
			}
				
			
		}


		
		void Terrain::draw()
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, heightMapTextureID);
			//texture.BindTexture(1);

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