/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:57
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\window_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	window_dx9
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Window dx9
*********************************************************************/
#ifndef __WINDOW_DX9_H__
#define __WINDOW_DX9_H__

#include "sims.h"
#include "common_dx9.h"
#include "renderer_caps_dx9.h"
#include "platform/windows/windows_window_base.h"

namespace sims
{
	namespace dx9
	{
		class Window : public WindowBase
		{
		public:
			virtual void OnCreate()
			{
				SetupD3D();
				SetupD3DDevice(GetHandle(), width_, height_, wndmode_);

				InitRendererCaps();
				g_RendererCaps.Dump();
			}

			virtual void OnDestroy()
			{
				DestoryDirectX();
			}
		};
	}
}

#endif