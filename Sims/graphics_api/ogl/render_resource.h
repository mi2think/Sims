/********************************************************************
	created:	2018/07/19
	created:	19:7:2018   17:07
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\render_resource.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_render_resource
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Render Resource
*********************************************************************/
#pragma once

#include "graphics/hw/render_resource.h"
#include "common.h"

namespace sims
{
	namespace ogl
	{
		class OGLTextureResource : public TextureResource
		{
		public:
			OGLTextureResource();
			~OGLTextureResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
			virtual void OnSamplerStatusUpdated() override;
		private:
			void InternalReleaseResource();

			GLuint resource_;
		};

		class OGLVertexBufferResource : public VertexBufferResource
		{
		public:
			OGLVertexBufferResource();
			~OGLVertexBufferResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		private:
			void InternalReleaseResource();

			GLuint vbo_;
		};

		class OGLVertexArrayResource : public VertexArrayResource
		{
		public:
			OGLVertexArrayResource();
			~OGLVertexArrayResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		private:
			void InternalReleaseResource();

			GLuint vao_;
		};

		class OGLIndexBufferResource : public IndexBufferResource
		{
		public:
			OGLIndexBufferResource();
			~OGLIndexBufferResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		private:
			void InternalReleaseResource();

			GLuint ibo_;
		};

		class OGLShaderResource : public ShaderResource
		{
		public:
			OGLShaderResource();
			~OGLShaderResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();

			GLuint GetResource() const { return resource_; }
		private:
			void InternalReleaseResource();

			GLuint resource_;
		};

		class OGLProgramResource : public ProgramResource
		{
		public:
			OGLProgramResource();
			~OGLProgramResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();

			virtual void SetUniform(const char* name, const void* data, uint32 dataSize);
		private:
			void AnalyseUniforms();
			void InternalReleaseResource();

			GLuint prog_;

			struct UniformVar
			{
				string name;
				GLuint index;
				GLint type;
				GLint size;
				GLuint loc;
			};
			vector<UniformVar> uniforms_;
		};
	}
}

