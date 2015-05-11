#include "ModelShader.h"

namespace gearengine
{
	namespace graphics
	{
		ModelShader::ModelShader()
		{
			mTotalShaders = 0;
			mShaders[VERTEX_SHADER] = 0;
			mShaders[FRAGMENT_SHADER] = 0;
			mShaders[GEOMETRY_SHADER] = 0;
			mAttributeList.clear();
			mUniformList.clear();
		}


		ModelShader::~ModelShader()
		{
			mAttributeList.clear();
			mUniformList.clear();
		}


		void ModelShader::loadShader(GLenum shaderType, const std::string& shaderSource)
		{
			GLuint shader = glCreateShader(shaderType);

			const char * ptmp = shaderSource.c_str();
			glShaderSource(shader, 1, &ptmp, NULL);

			//check whether the shader loads fine
			GLint status;
			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) 
			{
				GLint infoLogLength;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
				GLchar *infoLog = new GLchar[infoLogLength];
				glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
				std::cerr << "Compile log: " << infoLog << std::endl;
				delete[] infoLog;
			}
			mShaders[mTotalShaders++] = shader;
		}


		void ModelShader::loadFile(GLenum shaderType, const std::string& filename)
		{
			std::ifstream fp;
			fp.open(filename.c_str(), std::ios_base::in);
			if (fp) 
			{
				std::string line, buffer;
				while (getline(fp, line)) 
				{
					buffer.append(line);
					buffer.append("\r\n");
				}
				//copy to source
				loadShader(shaderType, buffer);
			}
			else
				std::cerr << "Error loading shader: " << filename << std::endl;		
		}


		void ModelShader::linkProgram()
		{
			mProgram = glCreateProgram();
			if (mShaders[VERTEX_SHADER] != 0)
				glAttachShader(mProgram, mShaders[VERTEX_SHADER]);
			if (mShaders[FRAGMENT_SHADER] != 0)
				glAttachShader(mProgram, mShaders[FRAGMENT_SHADER]);
			if (mShaders[GEOMETRY_SHADER] != 0)
				glAttachShader(mProgram, mShaders[GEOMETRY_SHADER]);

			//link and check whether the program links fine
			GLint status;
			glLinkProgram(mProgram);
			glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
			if (status == GL_FALSE) {
				GLint infoLogLength;

				glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
				GLchar *infoLog = new GLchar[infoLogLength];
				glGetProgramInfoLog(mProgram, infoLogLength, NULL, infoLog);
				std::cerr << "Link log: " << infoLog << std::endl;
				delete[] infoLog;
			}

			glDeleteShader(mShaders[VERTEX_SHADER]);
			glDeleteShader(mShaders[FRAGMENT_SHADER]);
			glDeleteShader(mShaders[GEOMETRY_SHADER]);
		}


		void ModelShader::DeleteShaderProgram()
		{
			glDeleteProgram(mProgram);
		}


		void ModelShader::addAttribute(const std::string& attribute)
		{
			mAttributeList[attribute] = glGetAttribLocation(mProgram, attribute.c_str());
		}


		void ModelShader::addUniform(const std::string& uniform)
		{
			mUniformList[uniform] = glGetUniformLocation(mProgram, uniform.c_str());
		}


		GLint ModelShader::getUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(mProgram, name);
		}


		void ModelShader::setUniform1f(const GLchar* name, float value)
		{
			glUniform1f(getUniformLocation(name), value);
		}


		void ModelShader::setUniform1fv(const GLchar* name, float* value, int count)
		{
			glUniform1fv(getUniformLocation(name), count, value);
		}


		void ModelShader::setUniform1i(const GLchar* name, int value)
		{
			glUniform1i(getUniformLocation(name), value);
		}


		void ModelShader::setUniform1iv(const GLchar* name, int* value, int count)
		{
			glUniform1iv(getUniformLocation(name), count, value);
		}


		void ModelShader::setUniform2f(const GLchar* name, const glm::vec2& vector)
		{
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}


		void ModelShader::setUniform2i(const GLchar* name, int value1, int value2)
		{
			glUniform2i(getUniformLocation(name), value1, value2);
		}


		void ModelShader::setUniform3f(const GLchar* name, const glm::vec3& vector)
		{
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
		}


		void ModelShader::setUniform4f(const GLchar* name, const glm::vec4& vector)
		{
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}


		void ModelShader::setUniformMat4(const GLchar* name, const glm::mat4& matrix)
		{
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}


		void ModelShader::setModelAndNormalMatrix(const GLchar* modelName, const GLchar* normalName, const glm::mat4& modelMatrix)
		{
			setUniformMat4(modelName, modelMatrix);
			setUniformMat4(normalName, glm::transpose(glm::inverse(modelMatrix)));
		}


		void ModelShader::enable() const
		{
			glUseProgram(mProgram);
		}


		void ModelShader::disable() const
		{
			glUseProgram(0);
		}
	}
}