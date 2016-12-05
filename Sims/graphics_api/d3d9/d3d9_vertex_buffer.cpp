/********************************************************************
	created:	2016/11/30
	created:	30:11:2016   21:15
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_vertex_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_vertex_buffer
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	D3D9 Vertex Buffer
*********************************************************************/
#include "d3d9_vertex_buffer.h"

namespace sims
{
	namespace d3d9
	{
		D3D9VertexBuffer::D3D9VertexBuffer()
			: VertexBuffer()
			, vb_(nullptr)
			, decl_(nullptr)
		{}

		D3D9VertexBuffer::D3D9VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount)
			: VertexBuffer(vertexDecl, vertexCount)
			, vb_(nullptr)
			, decl_(nullptr)
		{
		}

		D3D9VertexBuffer::~D3D9VertexBuffer()
		{
			SAFE_RELEASE(vb_);
			SAFE_RELEASE(decl_);
		}

		void D3D9VertexBuffer::HWUpdateVertexBuffer()
		{
			if (!decl_)
			{
				uint32 count = vertexDecl_->GetStreamCount();
				TBuffer<D3DVERTEXELEMENT9> elements(count + 1); // +1 for D3DDECL_END
				for (uint32 i = 0; i < count; ++i)
				{
					FillD3DVertexElement(&elements[i], vertexDecl_->GetStream(i));
				}
				elements[count] = D3DDECL_END();
				CHECK_HR = g_pD3DD->CreateVertexDeclaration(elements.GetData(), &decl_);
			}
			if (!vb_)
			{
				CHECK_HR = g_pD3DD->CreateVertexBuffer(vertexCount_ * vertexDecl_->GetStride(),
					D3DUSAGE_WRITEONLY,
					0, // using vertex decl
					D3DPOOL_MANAGED,
					&vb_,
					0);
			}
		}

		void D3D9VertexBuffer::HWBindVertexBuffer()
		{

		}

		void D3D9VertexBuffer::HWDeleteVertexBuffer()
		{

		}
	}
}