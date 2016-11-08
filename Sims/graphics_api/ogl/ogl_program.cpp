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
	}
}