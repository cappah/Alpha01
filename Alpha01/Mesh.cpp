#include "Mesh.h"


namespace gearengine
{
	namespace model
	{
		Mesh::Mesh()
		{
		}


		Mesh::Mesh(std::vector<Vertex> verts, std::vector<GLuint> ind, std::vector<Texture> texs, graphics::ModelShader& shader)
			: mVertices(verts), mIndices(ind), mTextures(texs), mShader(shader)
		{
			setupMesh();
		}


		Mesh::~Mesh()
		{

		}


		void Mesh::draw()
		{
			GLuint diffuseNr = 1;
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

			mShader.setUniform1i("material.shininess", 16.0f);

			// Draw mesh
			glBindVertexArray(mVAO);
			glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			for (GLuint i = 0; i < mTextures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		}


		void Mesh::setupMesh()
		{
			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mVBO);
			glGenBuffers(1, &mEBO);

			glBindVertexArray(mVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);

			// Vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

			// Vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

			// Texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

			glBindVertexArray(0);
		}
	}
}