/********************************************************************
	created:	2016/10/23
	created:	23:10:2016   17:59
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_shader.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_shader
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	OGL Shader
*********************************************************************/
#include "ogl_shader.h"

namespace sims
{
	namespace ogl
	{
		OGLShader::OGLShader()
			: Shader()
			, so_(0)
		{}

		OGLShader::~OGLShader()
		{
			DeleteInternal();
		}

		bool OGLShader::IsValid() const
		{
			return so_ != 0;
		}

		bool OGLShader::Compile(ShaderDomain::Type type, const string& source)
		{
			ASSERT(type == ShaderDomain::Vertex || type == ShaderDomain::Fragment);
			ASSERT(!IsValid());

			type_ = type;
			source_ = source;

			GLuint so = 0;
			if (type == ShaderDomain::Vertex)
				so = glCreateShader(GL_VERTEX_SHADER);
			else if (type == ShaderDomain::Fragment)
				so = glCreateShader(GL_FRAGMENT_SHADER);

			gl_check_error("glCreateShader");

			const char* csource = source.c_str();
			GLint csize = source.size();
			glShaderSource(so, 1, (GLchar**)&csource, &csize);
			gl_check_error("glShaderSource");

			glCompileShader(so);
			gl_check_error("glCompileShader");

			GLint status;
			glGetShaderiv(so, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE)
			{
				GLint logSize = 0;
				glGetShaderiv(so, GL_INFO_LOG_LENGTH, &logSize);
				if (logSize > 1)
				{
					string log;
					log.resize(logSize);
					glGetShaderInfoLog(so, logSize, nullptr, &log[0]);

					LOG_ERROR("Shader Compile error:");
					LOG_ERROR("%s", log.c_str());

					glDeleteShader(so);
					return false;
				}
			}

			so_ = so;
			return true;
		}

		bool OGLShader::LoadBinary(ShaderDomain::Type, char*, uint32)
		{
			ASSERT(false && "we won't load separate shader program in ogl");
			return false;
		}

		void OGLShader::Delete()
		{
			DeleteInternal();
		}

		void OGLShader::DeleteInternal()
		{
			if (so_ != 0)
			{
				glDeleteShader(so_);
				so_ = 0;
			}
		}

		void OGLShader::Bind()
		{
			ASSERT(false && "we won't use separate shader program in ogl");
		}

		UniformLoc OGLShader::GetUniformLoc(const char*, UniformLoc)
		{
			ASSERT(false && "we won't use separate shader program in ogl");
			return 0;
		}
	}
}