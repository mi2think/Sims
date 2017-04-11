/********************************************************************
	created:	2017/04/12
	created:	12:4:2017   2:58
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_window.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_window
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	D3D9 Window
*********************************************************************/
#include "d3d9_window.h"
#include "d3d9_common.h"
#include "d3d9_renderer_caps.h"

namespace sims
{
	namespace d3d9
	{
		void Window::OnCreate()
		{
			SetupD3D();
			SetupD3DDevice((HWND)GetHandle(), width_, height_, wndmode_);

			InitRendererCaps();
			g_RendererCaps.Dump();
		}

		void Window::OnDestroy()
		{
			DestoryDirectX();
		}
	}
}
