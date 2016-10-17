/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:27
	filename: 	D:\Code\Sims\Sims\graphics_api\renderer_rhi.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	renderer_rhi
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Renderer RHI
*********************************************************************/
#ifndef __RENDERER_RHI_H__
#define __RENDERER_RHI_H__

#include "sims.h"
#include "core/color.h"

namespace sims
{
	namespace rhi
	{
		class RHIRenderer
		{
		public:
			virtual ~RHIRenderer() {}

			virtual void BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil) = 0;
			virtual void EndFrame() = 0;
			virtual void PresentFrame() = 0;

			virtual void UpdateTexture(Texture& texture, Recti* regions) = 0;
			virtual void DeleteTexture(RenderID id) = 0;
		};
	}
}

#endif