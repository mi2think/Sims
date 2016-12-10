/********************************************************************
	created:	2016/12/10
	created:	10:12:2016   22:02
	filename: 	D:\Code\Sims\Sims\graphics\render_resource.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	render_resource
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Render Resource
*********************************************************************/
#include "render_resource.h"
#include "vertex_buffer.h"
#include "math/rectangle.h"

namespace sims
{
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

	void TextureResource::SetUpdateRegions(const TBuffer<Recti>& regions)
	{
		regions_ = regions;
	}

	VertexBufferResource::VertexBufferResource()
		: vertexBuffer_(nullptr)
		, vertexDecl_(nullptr)
	{
	}

	void VertexBufferResource::Attach(void* vertexBuffer)
	{
		vertexBuffer_ = static_cast<VertexBuffer*>(vertexBuffer);
		vertexDecl_ = vertexBuffer_->GetVertexDeclaration().Get();
	}

	IndexBufferResource::IndexBufferResource()
		: indexBuffer_(nullptr)
	{}

	void IndexBufferResource::Attach(void* indexBuffer)
	{
		indexBuffer_ = static_cast<IndexBuffer*>(indexBuffer);
	}
}