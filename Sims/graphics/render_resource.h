/********************************************************************
	created:	2016/12/10
	created:	10:12:2016   21:49
	filename: 	D:\Code\Sims\Sims\graphics\render_resource.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	render_resource
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Render Resource
*********************************************************************/
#pragma once

#include "graphics_fwd.h"

namespace sims
{
	class RenderResource
	{
	public:
		virtual ~RenderResource();

		virtual void Attach(void*) {}

		virtual void UpdateResource() {}
		virtual void BindResource() const {}
		virtual void ReleaseResource() {}
	};

	class TextureResource : public RenderResource
	{
	public:
		TextureResource();

		void Attach(void* texture);

		void SetBindStage(uint32 bindStage);
		void SetUpdateRegions(const TBuffer<Recti>& regions);
		virtual void OnSamplerStatusUpdated() {}
	protected:
		Texture* texture_;
		uint32 bindStage_;
		TBuffer<Recti> regions_;
	};

	class VertexDeclarationResource : public RenderResource
	{
	public:
		VertexDeclarationResource();

		void Attach(void* vertexDecl);
	protected:
		VertexDeclaration* vertexDecl_;
	};

	class VertexBufferResource : public RenderResource
	{
	public:
		VertexBufferResource();

		void Attach(void* vertexBuffer);
	protected:
		VertexBuffer* vertexBuffer_;
		const VertexStream* vertexStream_;
	};

	class IndexBufferResource : public RenderResource
	{
	public:
		IndexBufferResource();

		void Attach(void* indexBuffer);
		uint32 GetIndexCount() const;
	protected:
		IndexBuffer* indexBuffer_;
	};

	class ShaderResource : public RenderResource
	{
	public:
		ShaderResource();

		void Attach(void* shader);

		template<typename T>
		void SetUniform(const char* name, const T& val) { SetUniform(name, &val, sizeof(val)); }
	protected:
		virtual void SetUniform(const char*, const void*, uint32) {}
		
		Shader* shader_;
	};

	class ProgramResource : public RenderResource
	{
	public:
		ProgramResource();

		void Attach(void* program);

		template<typename T>
		void SetUniform(const char* name, const T& val) { SetUniform(name, &val, sizeof(val)); }
	protected:
		virtual void SetUniform(const char*, const void*, uint32) {}

		Program* program_;
	};
}

