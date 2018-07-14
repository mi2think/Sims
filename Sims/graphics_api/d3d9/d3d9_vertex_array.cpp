/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   14:34
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_vertex_array.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_vertex_array
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Vertex Array
*********************************************************************/
#include "d3d9_vertex_array.h"
#include "graphics/vertex_array.h"

namespace sims
{
	namespace d3d9
	{
		D3D9VertexArrayResource::D3D9VertexArrayResource()
			: VertexArrayResource()
		{
		}

		D3D9VertexArrayResource::~D3D9VertexArrayResource()
		{
		}

		void D3D9VertexArrayResource::UpdateResource()
		{
			auto vertexDecl = vertexArray_->GetVertexDeclaration();
			ASSERT(vertexDecl);
			vertexDecl->Invalidate();

			auto count = vertexArray_->GetVertexBufferCount();
			for (uint32 i = 0; i < count; ++i)
			{
				auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
				ASSERT(vertexBuffer);
				vertexBuffer->Invalidate();
			}
		}

		void D3D9VertexArrayResource::BindResource() const
		{
			auto vertexDecl = vertexArray_->GetVertexDeclaration();
			vertexDecl->Bind();

			auto count = vertexArray_->GetVertexBufferCount();
			for (uint32 i = 0; i < count; ++i)
			{
				auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
				vertexBuffer->Bind();
			}
		}
		
		void D3D9VertexArrayResource::ReleaseResource()
		{
			auto vertexDecl = vertexArray_->GetVertexDeclaration();
			vertexDecl->Release();

			auto count = vertexArray_->GetVertexBufferCount();
			for (uint32 i = 0; i < count; ++i)
			{
				auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
				vertexBuffer->Release();
			}
		}
	}
}