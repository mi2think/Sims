/********************************************************************
	created:	2016/12/05
	created:	5:12:2016   20:09
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_index_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_index_buffer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Index Buffer
*********************************************************************/
#include "d3d9_index_buffer.h"
#include "graphics/index_buffer.h"

namespace sims
{
	namespace d3d9
	{
		template<typename TIndexType> D3DFORMAT ToD3DIndexFormat() { ASSERT(false && "error index type"); return D3DFMT_FORCE_DWORD;}
		template<> D3DFORMAT ToD3DIndexFormat<uint16>() { return D3DFMT_INDEX16; }
		template<> D3DFORMAT ToD3DIndexFormat<uint32>() { return D3DFMT_INDEX32; }


		D3D9IndexBufferResource::D3D9IndexBufferResource()
			: IndexBufferResource()
			, ib_(nullptr)
		{}

		D3D9IndexBufferResource::~D3D9IndexBufferResource()
		{
			ASSERT(! ib_);
		}

		void D3D9IndexBufferResource::UpdateResource()
		{
			if (!ib_)
			{
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((indexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				CHECK_HR = g_pD3DD->CreateIndexBuffer(sizeof(IndexType) * indexBuffer_->GetIndexCount(),
					D3DUSAGE_WRITEONLY,
					ToD3DIndexFormat<IndexType>(),
					pool,
					&ib_,
					nullptr);
			}

			// update index buffer
			auto L = indexBuffer_->Lock(LockFlags::LockRead);

			void* indices = nullptr;
			CHECK_HR = ib_->Lock(0,
				0,
				&indices,
				D3DLOCK_DISCARD); // discard for update entire index buffer
			memcpy(indices, L->GetData(), sizeof(IndexType) * indexBuffer_->GetIndexCount());
			CHECK_HR = ib_->Unlock();

			indexBuffer_->Unlock(L);
		}

		void D3D9IndexBufferResource::BindResource()
		{
			ASSERT(ib_);
			CHECK_HR = g_pD3DD->SetIndices(ib_);
		}

		void D3D9IndexBufferResource::ReleaseResource()
		{
			SAFE_RELEASE(ib_);
		}
	}
}