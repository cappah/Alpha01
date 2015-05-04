#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/glew.h>

namespace gearengine
{
	namespace model
	{
		struct VERTEX_BASE
		{
			GLfloat xyz[3];
			GLfloat st[2];
			GLuint points[3];
			GLfloat normal[3];

			bool hasTexture = false;
			std::string texPath;

			std::vector<GLfloat> vertices;
			std::vector<GLuint> indices;
			std::vector<GLfloat> texCoords;

			GLuint vbo;
			GLuint ibo;
			GLuint vto;
			GLuint tID;

		}vertex;

		std::vector<VERTEX_BASE> out_vertex;

		class GModel
		{
		public:
			GModel();
			~GModel();

			bool loadDAE(std::string file, int obj);
			void draw(int obj);

		private:
			GLuint obj_tot = 0;

		private:
			void buildVBO(bool hasTex, int obj);
		};
	}
}


