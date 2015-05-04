#include "shader.h"

namespace gearengine 
{ 
	namespace graphics 
	{
		Shader::Shader(const char* vertPath, const char* fragPath)
			: m_VertPath(vertPath), m_FragPath(fragPath)
		{
			mShaderType[VERTEX_SHADER] = 0;
			mShaderType[FRAGMENT_SHADER] = 0;
			mShaderType[GEOMETRY_SHADER] = 0;
			mAttributeList.clear();
			mUniformList.clear();

			//mProgramID = loadProgram();
		}


		Shader::~Shader()
		{
			mAttributeList.clear();
			mUniformList.clear();
			glDeleteProgram(mProgramID);
		}


		void Shader::loadProgram()
		{
			mProgramID = glCreateProgram();
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

			glAttachShader(mProgramID, loadShader(m_VertPath, mProgramID, vertex, GL_VERTEX_SHADER, "vertex"));
			glAttachShader(mProgramID, loadShader(m_FragPath, mProgramID, fragment, GL_FRAGMENT_SHADER, "fragment"));

			glLinkProgram(mProgramID);
			glValidateProgram(mProgramID);

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			
		}


		GLuint Shader::loadShader(const char* file, GLuint programID, GLuint shaderID, int shaderType, const char* type)
		{
			shaderID = glCreateShader(shaderType);

			std::string sourceString = read_file(file);

			const char* source = sourceString.c_str();

			glShaderSource(shaderID, 1, &source, NULL);
			glCompileShader(shaderID);

			GLint result;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(shaderID, length, &length, &error[0]);
				std::cout << "Failed to compile " << type << " shader!" << std::endl << &error[0] << std::endl;
				glDeleteShader(shaderID);
				return 0;
			}
			return shaderID;
		}

	
		void Shader::addAttribute(const std::string& attribute)
		{
			mAttributeList[attribute] = glGetAttribLocation(mProgramID, attribute.c_str());
		}


		void Shader::addUniform(const std::string& uniform)
		{
			mUniformList[uniform] = glGetUniformLocation(mProgramID, uniform.c_str());
		}


		GLint Shader::getUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(mProgramID, name);
		}


		void Shader::setUniform1f(const GLchar* name, float value)
		{
			glUniform1f(getUniformLocation(name), value);
		}


		void Shader::setUniform1fv(const GLchar* name, float* value, int count)
		{
			glUniform1fv(getUniformLocation(name), count, value);
		}


		void Shader::setUniform1i(const GLchar* name, int value)
		{
			glUniform1i(getUniformLocation(name), value);
		}


		void Shader::setUniform1iv(const GLchar* name, int* value, int count)
		{
			glUniform1iv(getUniformLocation(name), count, value);
		}


		void Shader::setUniform2f(const GLchar* name, const glm::vec2& vector)
		{
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}


		void Shader::setUniform2i(const GLchar* name, int value1, int value2)
		{
			glUniform2i(getUniformLocation(name), value1, value2);
		}


		void Shader::setUniform3f(const GLchar* name, const glm::vec3& vector)
		{
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
		}


		void Shader::setUniform4f(const GLchar* name, const glm::vec4& vector)
		{
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}


		void Shader::setUniformMat4(const GLchar* name, const glm::mat4& matrix)
		{
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}


		void Shader::enable() const
		{
			glUseProgram(mProgramID);
		}


		void Shader::disable() const
		{
			glUseProgram(0);
		}

	} 
}