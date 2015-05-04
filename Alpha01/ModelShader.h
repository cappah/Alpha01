#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>

namespace gearengine
{
	namespace graphics
	{
		class ModelShader
		{
		public:
			ModelShader();
			~ModelShader();

			
			void loadFile(GLenum shaderType, const std::string& filename);

			void enable() const;
			void disable() const;

			void addAttribute(const std::string& attribute);
			void addUniform(const std::string& uniform);
			void setUniform1f(const GLchar* name, float value);
			void setUniform1fv(const GLchar* name, float* value, int count);
			void setUniform1i(const GLchar* name, int value);
			void setUniform1iv(const GLchar* name, int* value, int count);
			void setUniform2f(const GLchar* name, const glm::vec2& vector);
			void setUniform2i(const GLchar* name, int value1, int value2);
			void setUniform3f(const GLchar* name, const glm::vec3& vector);
			void setUniform4f(const GLchar* name, const glm::vec4& vector);
			void setUniformMat4(const GLchar* name, const glm::mat4& matrix);

			GLuint operator[](const std::string& attribute) { return mAttributeList[attribute]; }
			GLuint operator()(const std::string& uniform) { return mUniformList[uniform]; }
			void linkProgram();
			void DeleteShaderProgram();

		private:
			enum ShaderType
			{
				VERTEX_SHADER,
				FRAGMENT_SHADER,
				GEOMETRY_SHADER
			};

			GLuint mProgram;
			GLuint mShaders[3];
			int mTotalShaders;
			std::map<std::string, GLuint> mAttributeList;
			std::map<std::string, GLuint> mUniformList;

		private:
			void loadShader(GLenum shaderType, const std::string& shaderSource);
			
			GLint getUniformLocation(const GLchar* name);
		};

	}
}