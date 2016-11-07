/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:13
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_renderer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_renderer
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	D3D9 Renderer
*********************************************************************/
#include "d3d9_renderer.h"
#include "d3d9_common.h"
#include "graphics/texture.h"

namespace sims
{
	namespace d3d9
	{
		void D3D9Renderer::BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			DWORD flags = 0;
			if ((clearFlags & ClearFlags::Color) != 0)
				flags |= D3DCLEAR_TARGET;
			if ((clearFlags & ClearFlags::Depth) != 0)
				flags |= D3DCLEAR_ZBUFFER;
			if ((clearFlags & ClearFlags::Stencil) != 0)
				flags |= D3DCLEAR_STENCIL;

			CHECK_HR = g_pD3DD->Clear(0, 0, flags, color.value, depth, stencil);
			CHECK_HR = g_pD3DD->BeginScene();
		}

		void D3D9Renderer::EndFrame()
		{
			CHECK_HR = g_pD3DD->EndScene();
		}

		void D3D9Renderer::PresentFrame()
		{
			CHECK_HR = g_pD3DD->Present(0, 0, 0, 0);
		}
	}
}