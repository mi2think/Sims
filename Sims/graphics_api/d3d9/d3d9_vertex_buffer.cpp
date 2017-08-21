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
#include "graphics/vertex_buffer.h"

namespace sims
{
	namespace d3d9
	{
		D3D9VertexBufferResource::D3D9VertexBufferResource()
			: VertexBufferResource()
			, vb_(nullptr)
			, decl_(nullptr)
		{}

		D3D9VertexBufferResource::~D3D9VertexBufferResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexBufferResource::UpdateResource()
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
				VERIFYD3DRESULT(g_pD3DD->CreateVertexDeclaration(elements.GetData(), &decl_));
			}
			if (!vb_)
			{
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((vertexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				VERIFYD3DRESULT(g_pD3DD->CreateVertexBuffer(vertexBuffer_->GetVertexCount() * vertexDecl_->GetStride(),
					D3DUSAGE_WRITEONLY,
					0, // using vertex decl
					pool,
					&vb_,
					0));
			}

			// update vertex buffer
			const IndexRange& range = vertexBuffer_->GetInvalidRange();
			auto L = vertexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			
			void* vertices = nullptr;
			VERIFYD3DRESULT(vb_->Lock(
				L->GetOffset(),
				L->GetCount(),
				&vertices,
				D3DLOCK_DISCARD)); // discard for update entire vertex buffer
			memcpy(vertices, L->GetLockData(), L->GetCount() * vertexDecl_->GetStride());
			VERIFYD3DRESULT(vb_->Unlock());
			
			vertexBuffer_->Unlock(L);
		}

		void D3D9VertexBufferResource::BindResource() const
		{
			ASSERT(decl_ && vb_);

			VERIFYD3DRESULT(g_pD3DD->SetVertexDeclaration(decl_));
			VERIFYD3DRESULT(g_pD3DD->SetStreamSource(0, vb_, 0, vertexDecl_->GetStride()));
		}

		void D3D9VertexBufferResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexBufferResource::InternalReleaseResource()
		{
			SAFE_RELEASE(decl_);
			SAFE_RELEASE(vb_);
		}
	}
}