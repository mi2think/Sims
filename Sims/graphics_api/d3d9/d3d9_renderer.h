/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:35
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_renderer.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_renderer
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	D3D9 Renderer
*********************************************************************/
#ifndef __RENDERER_DX9_H__
#define __RENDERER_DX9_H__

#include "graphics_api/rhi/rhi_renderer.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9Renderer : public rhi::RHIRenderer
		{
		public:
			virtual void BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil);
			virtual void EndFrame();
			virtual void PresentFrame();
		};
	}
}



#endif
