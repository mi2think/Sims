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
		OGLProgramResource::OGLProgramResource()
			: ProgramResource()
			, prog_(0)
		{
		}

		OGLProgramResource::~OGLProgramResource()
		{
			InternalReleaseResource();
		}

		void OGLProgramResource::UpdateResource() 
		{
			ReleaseResource();

			prog_ = glCreateProgram();
			for (int i = 0; i < ShaderDomain::Max; ++i)
			{
				auto type = ShaderDomain::Type(i);
				auto shader = program_->GetShader(type);
				if (! shader)
					continue;

				auto& shaderRes = shader->HWResource();
				if (shaderRes)
				{
					auto oglShaderRes = shaderRes.As<OGLShaderResource>();
					glAttachShader(prog_, oglShaderRes->GetResource());
				}
			}
		
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
				}
				glDeleteProgram(prog_);
				prog_ = 0;
				return;
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
				}
				glDeleteProgram(prog_);
				prog_ = 0;
			}

			AnalyseUniforms();
		}

		void OGLProgramResource::BindResource() const
		{
			ASSERT(prog_ != 0);
			glUseProgram(prog_);
		}

		void OGLProgramResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLProgramResource::InternalReleaseResource()
		{
			if (prog_)
			{
				glDeleteProgram(prog_);
				prog_ = 0;
			}
			uniforms_.clear();
		}

		void OGLProgramResource::AnalyseUniforms()
		{
			GLint uniformCount;
			glGetProgramiv(prog_, GL_ACTIVE_UNIFORMS, &uniformCount);

			vector<GLint> indices;
			vector<GLint> types;
			vector<GLint> sizes;
			vector<GLint> nameLengths;
			AllocIndicesByRange(indices, IndexRange(0, uniformCount));
			AllocIndicesByRange(types, IndexRange(0, uniformCount));
			AllocIndicesByRange(sizes, IndexRange(0, uniformCount));
			AllocIndicesByRange(nameLengths, IndexRange(0, uniformCount));
			glGetActiveUniformsiv(prog_, uniformCount, (const GLuint*)&indices[0], GL_UNIFORM_TYPE, &types[0]);
			glGetActiveUniformsiv(prog_, uniformCount, (const GLuint*)&indices[0], GL_UNIFORM_SIZE, &sizes[0]);
			glGetActiveUniformsiv(prog_, uniformCount, (const GLuint*)&indices[0], GL_UNIFORM_NAME_LENGTH, &nameLengths[0]);
			gl_check_error("AnalyseUniforms - glGetActiveUniformsiv");

			Buffer buf;
			for (int i = 0; i < uniformCount; ++i)
			{
				UniformVar var;
				var.index = indices[i];
				var.size = sizes[i];
				var.type = types[i];
	
				buf.Resize(nameLengths[i]);
				GLsizei len = 0;
				glGetActiveUniformName(prog_, var.index, nameLengths[i], &len, &buf[0]);
				ASSERT(len == nameLengths[i]);
				var.name.assign(buf.GetData(), buf.GetSize());
				
				var.loc = glGetUniformLocation(prog_, buf.GetData());
				if (var.loc == GL_INVALID_VALUE)
				{
					LOG_ERROR("Program is now value generated by OpenGL");
				}
				else if (var.loc == GL_INVALID_OPERATION)
				{
					LOG_ERROR("Program is not a program or link failed");
				}
				
				uniforms_.push_back(var);
			}

			gl_check_error("AnalyseUniforms");
		}

		void OGLProgramResource::SetUniform(const char* name, const void* data, uint32 dataSize)
		{
			auto it = std::find_if(uniforms_.begin(), uniforms_.end(), [&name](const UniformVar& var) { return var.name == name; });
			ASSERT(it != uniforms_.end());

			switch (it->type)
			{
			case GL_FLOAT:		glUniform1fv(it->loc, 1, (const GLfloat*)data); ASSERT(sizeof(GLfloat) == dataSize); break;
			case GL_FLOAT_VEC2:	glUniform2fv(it->loc, 1, (const GLfloat*)data); ASSERT(sizeof(GLfloat) * 2 == dataSize); break;
			case GL_FLOAT_VEC3:	glUniform3fv(it->loc, 1, (const GLfloat*)data); ASSERT(sizeof(GLfloat) * 3 == dataSize); break;
			case GL_FLOAT_VEC4:	glUniform4fv(it->loc, 1, (const GLfloat*)data); ASSERT(sizeof(GLfloat) * 4 == dataSize); break;
			case GL_DOUBLE:		glUniform1dv(it->loc, 1, (const GLdouble*)data); ASSERT(sizeof(GLdouble) == dataSize); break;
			case GL_DOUBLE_VEC2:glUniform2dv(it->loc, 1, (const GLdouble*)data); ASSERT(sizeof(GLdouble) * 2 == dataSize); break;
			case GL_DOUBLE_VEC3:glUniform3dv(it->loc, 1, (const GLdouble*)data); ASSERT(sizeof(GLdouble) * 3 == dataSize); break;
			case GL_DOUBLE_VEC4:glUniform4dv(it->loc, 1, (const GLdouble*)data); ASSERT(sizeof(GLdouble) * 4 == dataSize); break;
			case GL_INT:		glUniform1iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) == dataSize); break;
			case GL_INT_VEC2:	glUniform2iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) * 2 == dataSize); break;
			case GL_INT_VEC3:	glUniform3iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) * 3 == dataSize); break;
			case GL_INT_VEC4:	glUniform4iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) * 4 == dataSize); break;
			case GL_FLOAT_MAT3: glUniformMatrix3fv(it->loc, 1, GL_FALSE, (const GLfloat*)data); ASSERT(sizeof(GLfloat) * 9 == dataSize); break;
			case GL_FLOAT_MAT4: glUniformMatrix4fv(it->loc, 1, GL_FALSE, (const GLfloat*)data); ASSERT(sizeof(GLfloat) * 16 == dataSize); break;
			case GL_SAMPLER_1D:	glUniform1iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) == dataSize); break;
			case GL_SAMPLER_2D:	glUniform1iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) == dataSize); break;
			case GL_SAMPLER_3D:	glUniform1iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) == dataSize); break;
			case GL_SAMPLER_CUBE: glUniform1iv(it->loc, 1, (const GLint*)data); ASSERT(sizeof(GLint) == dataSize); break;
			default:
				LOG_ERROR("not support set uniform type:%d", it->type);
				break;
			}

			gl_check_error("SetUniform");
		}
	}
}