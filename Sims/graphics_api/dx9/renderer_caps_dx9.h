/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:47
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\renderer_caps_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	renderer_caps_dx9
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Renderer Caps dx9
*********************************************************************/
#ifndef __RENDERER_CAPS_DX9_H__
#define __RENDERER_CAPS_DX9_H__

#include "common_dx9.h"

namespace sims
{
	namespace dx9
	{
		struct RendererCaps
		{
			uint32 adpCount;
			string adpDriverDesc; // default adapter
			string adpDriverVersion;

			uint32 VSVersion;
			uint32 PSVersion;

			void Dump();
		};

		extern RendererCaps g_RendererCaps;

		void InitRendererCaps();
	}
}

#endif
