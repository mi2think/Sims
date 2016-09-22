/********************************************************************
	created:	2016/09/23
	created:	23:9:2016   1:46
	filename: 	D:\Code\Sims\Sims\graphics_api\renderer_rhi.cpp
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	renderer_rhi
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Renderer RHI
*********************************************************************/
#include "renderer_rhi.h"

#define SIMS_RENDERER_DX9 1

#if SIMS_RENDERER_DX9
	#include "dx9/renderer_dx9.h"
#endif

namespace sims
{
	RHIRenderer* RHIRenderer::GetRenderer()
	{
#if SIMS_RENDERER_DX9
		static dx9::DX9Renderer s_renderer;
#endif
		return &s_renderer;
	}
}