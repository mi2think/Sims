/********************************************************************
	created:	2016/10/27
	created:	27:10:2016   16:12
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_renderer.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_renderer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Renderer
*********************************************************************/
#ifndef __OGL_RENDERER_H__
#define __OGL_RENDERER_H__

#include "graphics_api/rhi/rhi_renderer.h"

namespace sims
{
	namespace ogl
	{
		class OGLRenderer : public rhi::RHIRenderer
		{
		public:
			virtual void BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil);
			virtual void EndFrame();
			virtual void PresentFrame();

			virtual void UpdateTexture(Texture& texture, Recti* regions);
			virtual void BindTexture(Texture& texture, uint32 textureUnit);
			virtual void DeleteTexture(Texture& texture);
		};
	}
}

#endif