/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:17
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\renderer_caps_ogl.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	renderer_caps_ogl
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Renderer Caps ogl
*********************************************************************/
#ifndef __RENDERER_CAPS_OGL_H__
#define __RENDERER_CAPS_OGL_H__

#include "sims.h"

namespace sims
{
	namespace ogl
	{
		struct RendererCaps
		{
			const char* vendor;
			const char* renderer;
			const char* version;
			const char* glslVersion;

			void Dump();
		};

		extern RendererCaps g_RendererCaps;

		void InitRendererCaps();
	}
}

#endif
