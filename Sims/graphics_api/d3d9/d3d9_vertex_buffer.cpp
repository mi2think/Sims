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
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((storageFlags_ & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				CHECK_HR = g_pD3DD->CreateVertexBuffer(vertexCount_ * vertexDecl_->GetStride(),
					D3DUSAGE_WRITEONLY,
					0, // using vertex decl
					pool,
					&vb_,
					0);
			}

			// update vertex buffer
			void* vertices = nullptr;
			CHECK_HR = vb_->Lock(
				0,
				0,
				&vertices,
				D3DLOCK_DISCARD); // discard for update entire vertex buffer
			memcpy(vertices, vertexData_.GetData(), vertexCount_ * vertexDecl_->GetStride());
			CHECK_HR = vb_->Unlock();
		}

		void D3D9VertexBuffer::HWBindVertexBuffer()
		{
			ASSERT(decl_ && vb_);

			CHECK_HR = g_pD3DD->SetVertexDeclaration(decl_);
			CHECK_HR = g_pD3DD->SetStreamSource(0, vb_, 0, vertexDecl_->GetStride());
		}

		void D3D9VertexBuffer::HWDeleteVertexBuffer()
		{
			SAFE_RELEASE(decl_);
			SAFE_RELEASE(vb_);
		}
	}
}