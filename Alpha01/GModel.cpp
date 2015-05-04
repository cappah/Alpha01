#include "GModel.h"

namespace gearengine
{
	namespace model
	{
		GModel::GModel()
		{
		}


		GModel::~GModel()
		{
		}


		bool GModel::loadDAE(std::string file, int obj)
		{
			std::ifstream dae;
			std::string line;

			out_vertex.push_back(VERTEX_BASE());
			dae.open(file, std::ios::in);
			
			while (!dae.eof())
			{
				getline(dae, line);
				if (line.find("          <float_array id") != line.npos)
				{
					if (line.find("POSITION") != line.npos)
					{
						getline(dae, line);
						while (line.find("</float_array>") == line.npos)
						{
							sscanf(line.c_str(), "%f %f %f", &out_vertex[obj].xyz[0], &out_vertex[obj].xyz[1], &out_vertex[obj].xyz[2]);
							out_vertex[obj].vertices.push_back(out_vertex[obj].xyz[0]);
							out_vertex[obj].vertices.push_back(out_vertex[obj].xyz[1]);
							out_vertex[obj].vertices.push_back(out_vertex[obj].xyz[2]);

							getline(dae, line);
						}
					}
				}

				if (line.find("          <float_array id") != line.npos)
				{
					if (line.find("UV0") != line.npos)
					{
						getline(dae, line);
						while (line.find("</float_array>") == line.npos)
						{
							sscanf(line.c_str(), "%f %f", &out_vertex[obj].st[0], &out_vertex[obj].st[1]);
							out_vertex[obj].texCoords.push_back(out_vertex[obj].st[0]);
							out_vertex[obj].texCoords.push_back(out_vertex[obj].st[1]);
							getline(dae, line);
						}
					}
				}
				if (line.find("     <image id=") != line.npos)
				{
					int start = line.find("file:");
					int end = line.find("</init_from>");
					int len = end - (start + 7);
					out_vertex[obj].texPath = line.substr(start + 7, len);
					out_vertex[obj].hasTexture = true;
					//out_vertex[obj].tID = LOADTEXTURE();
					glEnable(GL_TEXTURE_2D);
				}

				if (line.find("        <triangles count=") != line.npos)
				{
					int start = line.find("<p>");
					int end = line.find("</p>");

					int length = end - (start + 3);

					std::string temp = line.substr(start + 3, length);
					std::stringstream ss(temp);

					GLuint tmp;

					while (ss >> tmp)
					{
						out_vertex[obj].indices.push_back(tmp);
					}
				}
			}
			dae.close();

			if (out_vertex[obj].hasTexture)
				buildVBO(true, obj);
			else
				buildVBO(false, obj);

			obj_tot += 1;
			return true;
		}


		void GModel::buildVBO(bool hasTex, int obj)
		{
			glGenBuffers(1, &out_vertex[obj].vbo);
			glBindBuffer(GL_ARRAY_BUFFER, out_vertex[obj].vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * out_vertex[obj].vertices.size(), &out_vertex[obj].vertices[0], GL_STATIC_DRAW);
			
			if (out_vertex[obj].hasTexture)
			{
				glBindBuffer(GL_ARRAY_BUFFER, out_vertex[obj].ibo);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindTexture(GL_TEXTURE_2D, out_vertex[obj].tID);
			}

			glGenBuffers(1, &out_vertex[obj].ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out_vertex[obj].ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_vertex[obj].indices.size(), &out_vertex[obj].indices[0], GL_STATIC_DRAW);

			if (hasTex)
			{
				glGenBuffers(1, &out_vertex[obj].vto);
				glBindBuffer(GL_ARRAY_BUFFER, out_vertex[obj].vto);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* out_vertex[obj].texCoords.size(), &out_vertex[obj].texCoords[0], GL_STATIC_DRAW);
			}
		}


		void GModel::draw(int obj)
		{
			glBindBuffer(GL_ARRAY_BUFFER, out_vertex[obj].vbo);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out_vertex[obj].ibo);
			glDrawElements(GL_TRIANGLES, out_vertex[obj].indices.size(), GL_UNSIGNED_INT, 0);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
}