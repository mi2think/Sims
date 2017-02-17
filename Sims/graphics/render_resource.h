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
		virtual void BindResource() {}
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

	class VertexBufferResource : public RenderResource
	{
	public:
		VertexBufferResource();

		void Attach(void* vertexBuffer);
	protected:
		VertexBuffer* vertexBuffer_;
		VertexDeclaration* vertexDecl_;
	};

	class IndexBufferResource : public RenderResource
	{
	public:
		IndexBufferResource();

		void Attach(void* indexBuffer);
	protected:
		IndexBuffer* indexBuffer_;
	};

	class ShaderResource : public RenderResource
	{
	public:
		ShaderResource();

		void Attach(void* shader);

		template<typename T>
		void SetConstant(const char* name, const T& val) { SetConstant(name, &val, sizeof(val)); }

		// d3d9 own
		virtual uint32 GetSamplerStage(const char* name) { ASSERT(false && "d3d9 own"); return 0; }

		// ogl own
		virtual void SetSamplerUnit(const char* name, uint32 imageUnit) { ASSERT(false && "ogl own"); };
		virtual uint32 GetSamplerUnit(const char* name) { ASSERT(false && "ogl own"); return 0; };
	protected:
		virtual void SetConstant(const char* name, const void* data, uint32 dataSize) = 0;
		
		Shader* shader_;
	};
}

