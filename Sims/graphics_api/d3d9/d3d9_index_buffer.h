/********************************************************************
	created:	2016/12/05
	created:	5:12:2016   20:06
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_index_buffer.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_index_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Index Buffer
*********************************************************************/
#pragma once

#include "graphics/index_buffer.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9IndexBuffer : public IndexBuffer
		{
		public:
			D3D9IndexBuffer();
			D3D9IndexBuffer(uint32 indexCount);
			D3D9IndexBuffer(uint32 indexCount, IndexType* data);
			~D3D9IndexBuffer();

			virtual void HWUpdateIndexBuffer();
			virtual void HWBindIndexBuffer();
			virtual void HWDeleteIndexBuffer();
		private:
			IDirect3DIndexBuffer9* ib_;
		};
	}
}
