/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:57
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_window.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_window
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	D3D9 Window
*********************************************************************/
#ifndef __WINDOW_DX9_H__
#define __WINDOW_DX9_H__

#include "sims.h"
#include "d3d9_common.h"
#include "d3d9_renderer_caps.h"
#include "platform/windows/windows_window_base.h"

namespace sims
{
	namespace d3d9
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