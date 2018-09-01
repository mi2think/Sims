/********************************************************************
	created:	2018/07/19
	created:	19:7:2018   17:10
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\render_resource.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	render_resource
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Render Resource
*********************************************************************/
#include "render_resource.h"

#include "graphics/hw/texture.h"
#include "graphics/hw/vertex_buffer.h"
#include "graphics/hw/vertex_array.h"
#include "graphics/hw/index_buffer.h"
#include "graphics/hw/shader.h"
#include "graphics/hw/program.h"

namespace sims
{
	namespace ogl
	{
		OGLTextureResource::OGLTextureResource()
			: TextureResource()
			, resource_(0)
		{
		}

		OGLTextureResource::~OGLTextureResource()
		{
			InternalReleaseResource();
		}

		void OGLTextureResource::UpdateResource()
		{
			// generate texture object
			if (resource_ == 0)
			{
				glGenTextures(1, &resource_);
			}

			glBindTexture(GL_TEXTURE_2D, resource_);

			for (uint32 i = 0; i < texture_->GetMipmapCount(); ++i)
			{
				ImageRef image = texture_->GetImage(i);
				if (!image)
					continue;
				uint32 width = image->GetWidth();
				uint32 height = image->GetHeight();
				const char* src = image->GetConstData();

				//TODO: support more pixel format & region
				auto format = image->GetFormat();
				ASSERT(format == PixelFormat::R8G8B8A8);

				glTexImage2D(GL_TEXTURE_2D,
					i,
					GL_RGBA,
					width,
					height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					(const GLvoid *)src);
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			// set sampling params of texture object
			OnSamplerStatusUpdated();

			gl_check_error("OGLRenderer::UpdateTexture");
		}

		void OGLTextureResource::BindResource() const
		{
			glActiveTexture(GL_TEXTURE0 + bindStage_);
			glBindTexture(GL_TEXTURE_2D, resource_);
			gl_check_error("OGLRenderer::BindTexture");
		}

		void OGLTextureResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLTextureResource::InternalReleaseResource()
		{
			if (resource_ != 0)
			{
				glDeleteTextures(1, &resource_);
				resource_ = 0;
			}
		}

		void OGLTextureResource::OnSamplerStatusUpdated()
		{
			glBindTexture(GL_TEXTURE_2D, resource_);

			// set sampling params of texture object
			const auto& samplerStatus = texture_->GetSamplerStatus();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLTextureFilterType(samplerStatus.GetFilterMin()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLTextureFilterType(samplerStatus.GetFilterMin()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLTextureWrapType(samplerStatus.GetWrapS()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLTextureWrapType(samplerStatus.GetWrapT()));
			auto color = samplerStatus.GetBorderColor().GetRGBAVector4();
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&color);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//////////////////////////////////////////////////////////////////////////

		OGLVertexBufferResource::OGLVertexBufferResource()
			: VertexBufferResource()
			, vbo_(0)
		{}

		OGLVertexBufferResource::~OGLVertexBufferResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexBufferResource::UpdateResource()
		{
			const IndexRange& range = vertexBuffer_->GetInvalidRange();
			auto L = vertexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			uint32 dataSize = L->GetCount() * vertexStream_->GetStride();
			uint32 dataOffset = L->GetOffset() * vertexStream_->GetStride();

			if (vbo_ == 0)
			{
				ASSERT(dataOffset == 0);
				glGenBuffers(1, &vbo_);
				gl_check_error("OGLVertexBufferResource::UpdateResource - glGenBuffers");

				GLenum usage = GL_STATIC_DRAW;
				if ((vertexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					usage = GL_DYNAMIC_DRAW;

				glBindBuffer(GL_ARRAY_BUFFER, vbo_);
				glBufferData(GL_ARRAY_BUFFER, dataSize, L->GetData(), usage);
				gl_check_error("OGLVertexBufferResource::UpdateResource - glBufferData");
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, vbo_);
				glBufferSubData(GL_ARRAY_BUFFER, dataOffset, dataSize, L->GetData());
				gl_check_error("OGLVertexBufferResource::UpdateResource - glBufferSubData");
			}

			vertexBuffer_->Unlock(L);
		}

		void OGLVertexBufferResource::BindResource() const
		{
			ASSERT(vbo_ != 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_);

			// bind stream
			uint32 count = vertexStream_->GetVertexElementCount();
			uint32 attriBaseIndex = vertexStream_->GetAttriBaseIndex();
			for (uint32 i = 0; i < count; ++i)
			{
				const VertexElement& vertexElement = vertexStream_->GetVertexElement(i);

				uint32 attriIndex = attriBaseIndex + i;
				glEnableVertexAttribArray(attriIndex);
				gl_check_error("glEnableVertexAttribArray");

				glVertexAttribPointer(attriIndex,
					vertexElement.GetComponentCount(),
					ToGLElementType(vertexElement.GetComponentType()),
					GL_FALSE,
					vertexStream_->GetStride(),
					(const void*)vertexElement.GetOffset());
				gl_check_error("glVertexAttribPointer");
			}
		}

		void OGLVertexBufferResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexBufferResource::InternalReleaseResource()
		{
			if (vbo_ != 0)
			{
				glDeleteBuffers(1, &vbo_);
				vbo_ = 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		OGLVertexArrayResource::OGLVertexArrayResource()
			: VertexArrayResource()
			, vao_(0)
		{
		}

		OGLVertexArrayResource::~OGLVertexArrayResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexArrayResource::UpdateResource()
		{
			if (OGLCapsInfo::SupportVertexArrayObjects())
			{
				if (vao_ == 0)
				{
					glGenVertexArrays(1, &vao_);
					glBindVertexArray(vao_);

					// configure attributes in VAO
					auto count = vertexArray_->GetVertexBufferCount();
					for (uint32 i = 0; i < count; ++i)
					{
						auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
						vertexBuffer->Invalidate();
						vertexBuffer->Bind();
					}

					gl_check_error("OGLVertexArrayResource::UpdateResource");
				}
			}
			// fall back
			else
			{
				auto count = vertexArray_->GetVertexBufferCount();
				for (uint32 i = 0; i < count; ++i)
				{
					auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
					vertexBuffer->Invalidate();
				}
			}
		}

		void OGLVertexArrayResource::BindResource() const
		{
			if (OGLCapsInfo::SupportVertexArrayObjects())
			{
				ASSERT(vao_ != 0);
				glBindVertexArray(vao_);

				gl_check_error("OGLVertexArrayResource::BindResource");
			}
			// fall back
			else
			{
				auto count = vertexArray_->GetVertexBufferCount();
				for (uint32 i = 0; i < count; ++i)
				{
					auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
					vertexBuffer->Bind();
				}
			}
		}

		void OGLVertexArrayResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexArrayResource::InternalReleaseResource()
		{
			if (vao_ != 0)
			{
				glDeleteBuffers(1, &vao_);
				vao_ = 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		OGLIndexBufferResource::OGLIndexBufferResource()
			: IndexBufferResource()
			, ibo_(0)
		{}

		OGLIndexBufferResource::~OGLIndexBufferResource()
		{
			InternalReleaseResource();
		}

		void OGLIndexBufferResource::UpdateResource()
		{
			const IndexRange& range = indexBuffer_->GetInvalidRange();
			auto L = indexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			uint32 dataSize = L->GetCount() * sizeof(IndexType);
			uint32 dataOffset = L->GetOffset() * sizeof(IndexType);

			if (ibo_ == 0)
			{
				ASSERT(dataOffset == 0);
				glGenBuffers(1, &ibo_);
				gl_check_error("OGLIndexBufferResource::UpdateResource - glGenBuffers");

				GLenum usage = GL_STATIC_DRAW;
				if ((indexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					usage = GL_DYNAMIC_DRAW;

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, L->GetData(), usage);
				gl_check_error("OGLIndexBufferResource::UpdateResource - glBufferData");
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, dataOffset, dataSize, L->GetData());
				gl_check_error("OGLIndexBufferResource::UpdateResource - glBufferSubData");
			}

			indexBuffer_->Unlock(L);
		}

		void OGLIndexBufferResource::BindResource() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
		}

		void OGLIndexBufferResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLIndexBufferResource::InternalReleaseResource()
		{
			if (ibo_ != 0)
			{
				glDeleteBuffers(1, &ibo_);
				ibo_ = 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////

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
				if (!shader)
					continue;

				auto& shaderRes = shader->Resource();
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
			if (uniformCount == 0)
				return;

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
				ASSERT(len + 1 == nameLengths[i]); 	// nameLength count null character.
				var.name.assign(buf.GetData(), len);

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
