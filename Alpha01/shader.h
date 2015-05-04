#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <map>

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "fileutils.h"

namespace gearengine 
{ 
	namespace graphics 
	{

		class Shader
		{
		public:
			Shader(){}
			Shader(const char* vertPath, const char* fragPath);
			~Shader();

			void loadProgram();

			void setUniform1f(const GLchar* name, float value);
			void setUniform1fv(const GLchar* name, float* value, int count);
			void setUniform1i(const GLchar* name, int value);
			void setUniform1iv(const GLchar* name, int* value, int count);
			void setUniform2f(const GLchar* name, const glm::vec2& vector);
			void setUniform2i(const GLchar* name, int value1, int value2);
			void setUniform3f(const GLchar* name, const glm::vec3& vector);
			void setUniform4f(const GLchar* name, const glm::vec4& vector);
			void setUniformMat4(const GLchar* name, const glm::mat4& matrix);

			void enable() const;
			void disable() const;

			void addAttribute(const std::string& attribute);
			void addUniform(const std::string& uniform);

			inline GLuint getProgramID() const { return mProgramID; }

			GLuint operator[](const std::string& attribute) { return mAttributeList[attribute]; }
			GLuint operator()(const std::string& uniform) { return mUniformList[uniform]; }

		private:
			const char* m_VertPath;
			const char* m_FragPath;
			GLuint mProgramID;

			GLuint mShaderType[3];
			std::map<std::string, GLuint> mAttributeList;
			std::map<std::string, GLuint> mUniformList;
			int mTotalShaders;

			enum ShaderType 
			{
				VERTEX_SHADER,
				FRAGMENT_SHADER,
				GEOMETRY_SHADER
			};

			

		private:
			
			GLuint loadShader(const char* file, GLuint programID, GLuint shaderID, int shaderType, const char* type);
			GLint getUniformLocation(const GLchar* name);
		};
	} 
}