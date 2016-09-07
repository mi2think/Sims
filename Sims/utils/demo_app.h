/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   23:47
	filename: 	D:\Code\Sims\Sims\utils\demo_app.h
	file path:	D:\Code\Sims\Sims\utils
	file base:	demo_app
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Demo App
*********************************************************************/
#ifndef __DEMO_APP_H__
#define __DEMO_APP_H__

#include "sims.h"

namespace sims
{
	class DemoAppBase
	{
	public:
		DemoAppBase() {}
		virtual ~DemoAppBase() {};

		// width, height, title, wndmode
		virtual void Create(int, int, const char*, bool) {}
		// title
		virtual void CreateFullScreen(const char*) {}

		virtual void OnCreate() {}
		virtual void OnUpdate(const Timestep&) {}
		virtual void OnRender(const Timestep&) {}
		virtual bool OnEvent(const Event&) { return false; }
		virtual void OnDestroy() {}
	};

	template<typename Window>
	class DemoApp : public DemoAppBase
	{
	public:
		DemoApp()
			: width_(0)
			, height_(0)
			, window_(new Window)
		{}

		virtual ~DemoApp()
		{}

		void Create(uint32 width, uint32 height, const char* title, bool wndmode = true)
		{
			window_->Create(width, height, title, wndmode);
			OnCreate();
		}

		void CreateFullScreen(const char* title)
		{
			window_->CreateFullScreen(title);
			OnCreate();
		}

		bool Loop() { return window_->Loop(); }

		virtual void OnCreate() override
		{
			window_->SetApp(this);
			width_ = window_->GetWidth();
			height_ = window_->GetHeight();
		}
	protected:
		uint32 width_;
		uint32 height_;
		Window* window_;
	};
}

#endif