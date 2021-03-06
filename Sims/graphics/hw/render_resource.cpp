/********************************************************************
	created:	2016/12/10
	created:	10:12:2016   22:02
	filename: 	D:\Code\Sims\Sims\graphics\hw\render_resource.cpp
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	render_resource
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Render Resource
*********************************************************************/
#include "render_resource.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "math/rectangle.h"

namespace sims
{
	IResourceOperation::IResourceOperation()
		: storageFlags_(StorageFlags::Local | StorageFlags::Hardware)
	{}

	IResourceOperation::~IResourceOperation()
	{}

	void IResourceOperation::Invalidate()
	{
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;

		if (!HWResource_)
			Create();
		else
			PreUpdate();

		HWResource_->Attach(this);
		HWResource_->UpdateResource();
	}

	void IResourceOperation::Bind()
	{
		ASSERT(HWResource_);
		HWResource_->BindResource();
	}

	void IResourceOperation::Release()
	{
		ASSERT(HWResource_);
		HWResource_->ReleaseResource();
		HWResource_ = nullptr;
	}

	void IResourceOperation::PreUpdate()
	{}
	//////////////////////////////////////////////////////////////////////////

	RenderResource::~RenderResource()
	{}

	TextureResource::TextureResource()
		: texture_(nullptr)
		, bindStage_(0)
	{}

	void TextureResource::Attach(void* texture)
	{
		texture_ = static_cast<Texture*>(texture);
	}

	void TextureResource::SetBindStage(uint32 bindStage)
	{
		bindStage_ = bindStage;
	}

	void TextureResource::SetUpdateRegions(const vector<Recti>& regions)
	{
		regions_ = regions;
	}

	void TextureResource::OnOperate(OPCODE Op, const void* Param)
	{
		switch (Op)
		{
		case OP_TEX_UPDATE_REGIONS:
			SetUpdateRegions(*((const vector<Recti>*)Param));
			break;
		case OP_TEX_BIND_STAGE:
			SetBindStage(*((const uint32*)Param));
			break;
		case OP_TEX_UPDATE_SAMPLE:
			OnSamplerStatusUpdated();
			break;
		}
	}

	VertexDeclarationResource::VertexDeclarationResource()
		: vertexDecl_(nullptr)
	{
	}

	void VertexDeclarationResource::Attach(void* vertexDecl)
	{
		vertexDecl_ = static_cast<VertexDeclaration*>(vertexDecl);
	}

	VertexBufferResource::VertexBufferResource()
		: vertexBuffer_(nullptr)
		, vertexStream_(nullptr)
	{
	}

	void VertexBufferResource::Attach(void* vertexBuffer)
	{
		vertexBuffer_ = static_cast<VertexBuffer*>(vertexBuffer);
		vertexStream_ = vertexBuffer_->GetVertexStream();
	}

	VertexArrayResource::VertexArrayResource()
		: vertexArray_(nullptr)
	{
	}

	void VertexArrayResource::Attach(void* vertexArray)
	{
		vertexArray_ = static_cast<VertexArray*>(vertexArray);
	}

	IndexBufferResource::IndexBufferResource()
		: indexBuffer_(nullptr)
	{}

	void IndexBufferResource::Attach(void* indexBuffer)
	{
		indexBuffer_ = static_cast<IndexBuffer*>(indexBuffer);
	}

	uint32 IndexBufferResource::GetIndexCount() const
	{
		return indexBuffer_->GetIndexCount();
	}

	ShaderResource::ShaderResource()
	{}

	void ShaderResource::Attach(void* shader)
	{
		shader_ = static_cast<Shader*>(shader);
	}

	ProgramResource::ProgramResource()
	{}

	void ProgramResource::Attach(void* program)
	{
		program_ = static_cast<Program*>(program);
	}
}