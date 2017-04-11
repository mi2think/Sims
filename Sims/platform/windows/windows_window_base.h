/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:46
	filename: 	D:\Code\Sims\Sims\platform\windows\windows_window_base.h
	file path:	D:\Code\Sims\Sims\platform\windows
	file base:	windows_window_base
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Window Base
*********************************************************************/
#ifndef __WINDOWS_WINDOW_BASE_H__
#define __WINDOWS_WINDOW_BASE_H__

#include "sims.h"
#include "core/timer.h"
#include "utils/demo_app.h"

namespace sims
{
	class WindowBase
	{
	public:
		WindowBase();
		~WindowBase();

		void Create(uint32 width, uint32 height, const char* title, bool wndmode);
		void CreateFullScreen(const char* title);
		bool MsgProc(void* hwnd, uint32 uMsg, uint64 wParam, uint64 lParam);
		bool Loop();

		uint32 GetWidth() const { return width_; }
		uint32 GetHeight() const { return height_; }
		void* GetHandle() const;

		void SetApp(DemoAppBase* app);

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
	protected:
		uint32 width_;
		uint32 height_;
		bool wndmode_;
		DemoAppBase* app_;
		Timer timer_;
	};
}

#endif