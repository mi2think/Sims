/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:27
	filename: 	D:\Code\Sims\Sims\graphics_api\hw_renderer.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	rhi_renderer
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Hardware Renderer
*********************************************************************/
#ifndef __RENDERER_HW_H__
#define __RENDERER_HW_H__

#include "graphics/graphics_fwd.h"
#include "core/color.h"

namespace sims
{
	namespace hw
	{
		class HWRenderer
		{
		public:
			virtual ~HWRenderer() {}

			virtual void BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil) = 0;
			virtual void EndFrame() = 0;
			virtual void PresentFrame() = 0;
		};
	}
}

#endif