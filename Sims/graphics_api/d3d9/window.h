/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:57
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\window.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	window
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	D3D9 Window
*********************************************************************/
#ifndef __WINDOW_DX9_H__
#define __WINDOW_DX9_H__

#include "platform/windows/windows_window_base.h"

namespace sims
{
	namespace d3d9
	{
		class Window : public WindowBase
		{
		public:
			virtual void OnCreate();
			virtual void OnDestroy();
		};
	}
}

#endif