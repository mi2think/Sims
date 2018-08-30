/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:47
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\renderer_caps.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_renderer_caps
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Renderer Caps
*********************************************************************/
#ifndef __RENDERER_CAPS_DX9_H__
#define __RENDERER_CAPS_DX9_H__

#include "common.h"

namespace sims
{
	namespace d3d9
	{
		struct RendererCaps
		{
			uint32 adpCount;
			string adpDriverDesc; // default adapter
			string adpDriverVersion;

			uint32 VSVersion;
			uint32 PSVersion;
			string VSVersionProfile;
			string PSVersionProfile;

			const char* GetVSVersionProfile() const { return VSVersionProfile.c_str(); }
			const char* GetPSVersionProfile() const { return PSVersionProfile.c_str(); }

			void Dump();
		};

		extern RendererCaps g_RendererCaps;

		void InitRendererCaps();
	}
}

#endif
