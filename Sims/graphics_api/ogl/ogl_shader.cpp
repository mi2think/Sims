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
#include "graphics/shader.h"

namespace sims
{
	namespace ogl
	{
		OGLShaderResource::OGLShaderResource()
			: ShaderResource()
			, resource_(0)
		{}

		OGLShaderResource::~OGLShaderResource()
		{
			InternalReleaseResource();
		}

		void OGLShaderResource::UpdateResource()
		{
			ReleaseResource();

			auto type = shader_->GetType();
			const auto& source = shader_->GetSource();

			GLuint resource = 0;
			if (type == ShaderDomain::Vertex)
				resource = glCreateShader(GL_VERTEX_SHADER);
			else if (type == ShaderDomain::Fragment)
				resource = glCreateShader(GL_FRAGMENT_SHADER);

			gl_check_error("glCreateShader");

			const char* csource = source.c_str();
			GLint csize = source.size();
			glShaderSource(resource, 1, (GLchar**)&csource, &csize);
			gl_check_error("glShaderSource");

			glCompileShader(resource);
			gl_check_error("glCompileShader");

			GLint status;
			glGetShaderiv(resource, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE)
			{
				GLint logSize = 0;
				glGetShaderiv(resource, GL_INFO_LOG_LENGTH, &logSize);
				if (logSize > 1)
				{
					Buffer log;
					log.Resize(logSize);
					glGetShaderInfoLog(resource, logSize, nullptr, log.GetData());

					LOG_ERROR("Shader Compile error:");
					LOG_ERROR("%s", log.GetData());

					glDeleteShader(resource);
					return;
				}
			}

			resource_ = resource;
		}

		void OGLShaderResource::BindResource() const
		{
			ASSERT(false && "we won't use separate shader program in ogl");
		}

		void OGLShaderResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLShaderResource::InternalReleaseResource()
		{
			if (resource_ != 0)
			{
				glDeleteShader(resource_);
				resource_ = 0;
			}
		}
	}
}