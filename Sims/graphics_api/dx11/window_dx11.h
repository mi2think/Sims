/********************************************************************
	created:	2016/10/09
	created:	9:10:2016   21:55
	filename: 	D:\Code\Sims\Sims\graphics_api\dx11\window_dx11.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx11
	file base:	window_dx11
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Window dx9
*********************************************************************/
#ifndef __WINDOW_DX11_H__
#define __WINDOW_DX11_H__

#include "sims.h"
#include "common_dx11.h"
#include "platform/windows/windows_window_base.h"

namespace sims
{
	namespace dx11
	{
		class Window : public WindowBase
		{
		public:
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