/********************************************************************
	created:	2016/10/09
	created:	9:10:2016   21:55
	filename: 	D:\Code\Sims\Sims\graphics_api\dx11\d3d11_window.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx11
	file base:	d3d11_window
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Window
*********************************************************************/
#ifndef __WINDOW_DX11_H__
#define __WINDOW_DX11_H__

#include "sims.h"
#include "d3d11_common.h"
#include "platform/windows/windows_window_base.h"

namespace sims
{
	namespace d3d11
	{
		class Window : public WindowBase
		{
		public:
			Window()
			{}

			virtual void OnCreate()
			{
				SetupD3DDevice(GetHandle(), width_, height_, wndmode_);
			}

			virtual void OnDestroy()
			{
				DestoryDirectX();
			}
		};
	}
}

#endif