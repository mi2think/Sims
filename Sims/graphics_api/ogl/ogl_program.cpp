/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   22:59
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_program.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_program
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	OGL Program
*********************************************************************/
#include "ogl_program.h"
#include "ogl_shader.h"

namespace sims
{
	namespace ogl
	{
		OGLProgram::OGLProgram()
			: Program()
			, prog_(glCreateProgram())
		{
		}

		OGLProgram::~OGLProgram()
		{
			if (prog_)
			{
				glDeleteProgram(prog_);
				prog_ = 0;
			}
		}

		void OGLProgram::AttachShader(const ShaderRef& shader)
		{
			ASSERT(prog_ != 0);
			Program::AttachShader(shader);
			
			auto oglShader = shader.As<OGLShader>();
			ASSERT(oglShader);

			glAttachShader(prog_, oglShader->GetShaderObj());
		}

		void OGLProgram::EndAttachShader()
		{
			ASSERT(prog_ != 0);

			glLinkProgram(prog_);

			GLint status;
			glGetProgramiv(prog_, GL_LINK_STATUS, &status);
			if (status != GL_TRUE)
			{
				GLint logSize = 0;
				glGetProgramiv(prog_, GL_INFO_LOG_LENGTH, &logSize);
				if (logSize > 1)
				{
					string log;
					log.resize(logSize);
					glGetProgramInfoLog(prog_, logSize, nullptr, &log[0]);

					LOG_ERROR("Link Program error:");
					LOG_ERROR("%s", log.c_str());

					glDeleteProgram(prog_);
					prog_ = 0;
					return;
				}
			}

			glValidateProgram(prog_);
			glGetProgramiv(prog_, GL_VALIDATE_STATUS, &status);
			if (status != GL_TRUE)
			{
				GLint logSize = 0;
				glGetProgramiv(prog_, GL_INFO_LOG_LENGTH, &logSize);
				if (logSize > 1)
				{
					string log;
					log.resize(logSize);
					glGetProgramInfoLog(prog_, logSize, nullptr, &log[0]);

					LOG_ERROR("Validate Program error:");
					LOG_ERROR("%s", log.c_str());

					glDeleteProgram(prog_);
					prog_ = 0;
				}
			}
		}

		void OGLProgram::BindProgram()
		{
			ASSERT(prog_ != 0);
			glUseProgram(prog_);
		}

		void OGLProgram::DeleteProgram()
		{
			if (prog_)
			{
				glDeleteProgram(prog_);
				prog_ = 0;
			}
		}

		UniformLoc OGLProgram::VSGetUniformLoc(const char* name, UniformLoc parent) const
		{
			return GetUniformLoc(name);
		}

		UniformLoc OGLProgram::FSGetUniformLoc(const char* name, UniformLoc parent) const
		{
			return GetUniformLoc(name);
		}

		UniformLoc OGLProgram::GetUniformLoc(const char* name) const
		{
			GLuint loc = glGetUniformLocation(prog_, name);
			if (loc == GL_INVALID_VALUE)
				LOG_ERROR("Program is now value generated by OpenGL");
			else if (loc == GL_INVALID_OPERATION)
				LOG_ERROR("Program is not a program or link failed");
			return (UniformLoc)loc;
		}

		void OGLProgram::VSBindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type)
		{
			return BindUniform(loc, data, count, type);
		}

		void OGLProgram::FSBindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type)
		{
			return BindUniform(loc, data, count, type);
		}

		void OGLProgram::BindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type)
		{
			GLint location = (GLint)loc;
			switch (type)
			{
			case DataType::Int:
				glUniform1iv(location, count, (const GLint*)data);
				break;
			case DataType::Int2:
				glUniform2iv(location, count, (const GLint*)data);
				break;
			case DataType::Int3:
				glUniform3iv(location, count, (const GLint*)data);
				break;
			case DataType::Int4:
				glUniform4iv(location, count, (const GLint*)data);
				break;
			case DataType::F32:
				glUniform1fv(location, count, (const GLfloat*)data);
				break;
			case DataType::Vec2:
				glUniform2fv(location, count, (const GLfloat*)data);
				break;
			case DataType::Vec3:
				glUniform3fv(location, count, (const GLfloat*)data);
				break;
			case DataType::Vec4:
				glUniform4fv(location, count, (const GLfloat*)data);
				break;
			case DataType::Mat3:
				glUniformMatrix3fv(location, count, GL_FALSE, (const GLfloat*)data);
				break;
			case DataType::Mat4:
				glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat*)data);
				break;
			case DataType::Sampler2D:
				break;
			case DataType::Color:
				{
					const Color& c = *(const Color*)data;
					auto v = c.GetRGBAVector4();
					glUniform4f(location, v.x, v.y, v.z, v.w);
				}
				break;
			case DataType::Unknown:
			default:
				ASSERT(false && "Unspported uniform type");
				break;
			}

			gl_check_error("OGLProgram::BindUniform");
		}
	}
}