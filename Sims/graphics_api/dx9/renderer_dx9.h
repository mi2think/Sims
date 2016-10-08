/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:35
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\renderer_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	renderer_dx9
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Renderer dx9
*********************************************************************/
#ifndef __RENDERER_DX9_H__
#define __RENDERER_DX9_H__

#include "sims.h"
#include "graphics_api/renderer_rhi.h"
#include <d3d9.h>

namespace sims
{
	namespace dx9
	{
		class DX9Renderer : public RHIRenderer
		{
		public:
			virtual void BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil);
			virtual void EndFrame();
			virtual void PresentFrame();

			virtual void UpdateTexture(Texture& texture, Recti* regions);
			virtual void DeleteTexture(RenderID id);
		};
	}
}



#endif
