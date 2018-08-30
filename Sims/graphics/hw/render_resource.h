/********************************************************************
	created:	2016/12/10
	created:	10:12:2016   21:49
	filename: 	D:\Code\Sims\Sims\graphics\hw\render_resource.h
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	render_resource
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Render Resource
*********************************************************************/
#pragma once

#include "graphics/graphics_fwd.h"

namespace sims
{
	// resource operation interface
	class IResourceOperation
	{
	public:
		IResourceOperation();
		virtual ~IResourceOperation();

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }

		// get resource
		RenderResourceRef& Resource() { return HWResource_; }

		// bind resource
		void Bind();

		// release resource
		void Release();

		// propagates changes on the resource to the renderer.
		//   you must call invalidate after modifying resource data,
		//   for it to be uploaded to GPU.
		virtual void Invalidate();
	protected:
		// create resource
		virtual void Create() = 0;
		// a chance before real upload to GPU.
		virtual void PreUpdate();

		uint32 storageFlags_;
		RenderResourceRef HWResource_;
	};

	// various render resource
	class RenderResource
	{
	public:
		virtual ~RenderResource();

		virtual void Attach(void*) {}

		virtual void UpdateResource() {}
		virtual void BindResource() const {}
		virtual void ReleaseResource() {}

		// for special case
		enum OPCODE
		{
			OP_NONE = 0,

			// texture
			OP_TEX_UPDATE_REGIONS = 1,
			OP_TEX_BIND_STAGE = 2,
			OP_TEX_UPDATE_SAMPLE = 3,
		};
		virtual void OnOperate(OPCODE, const void* = nullptr) {}
	};

	class TextureResource : public RenderResource
	{
	public:
		TextureResource();

		void Attach(void* texture);

		void OnOperate(OPCODE Op, const void* Parm);
	protected:
		void SetBindStage(uint32 bindStage);
		void SetUpdateRegions(const vector<Recti>& regions);
		virtual void OnSamplerStatusUpdated() {}

		Texture* texture_;
		uint32 bindStage_;
		vector<Recti> regions_;
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

	class VertexArrayResource : public RenderResource
	{
	public:
		VertexArrayResource();

		void Attach(void* vertexArray);
	protected:
		VertexArray* vertexArray_;
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

