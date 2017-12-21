/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   23:47
	filename: 	D:\Code\Sims\Sims\core\app.h
	file path:	D:\Code\Sims\Sims\core
	file base:	app
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	App
*********************************************************************/
#pragma once

#include "sims.h"

namespace sims
{
	void InitApp();
	void ExitApp();

	class AppBase
	{
	public:
		AppBase() {}
		virtual ~AppBase() {};

		// width, height, title, wndmode
		virtual void Create(int, int, const char*, bool) {}
		// title
		virtual void CreateFullScreen(const char*) {}
		virtual void Destroy() {}

		virtual void OnCreate() {}
		virtual void OnUpdate(const Timestep&) {}
		virtual void OnRender(const Timestep&) {}
		virtual bool OnEvent(const Event&) { return false; }
		virtual void OnDestroy() {}
	};

	template<typename Window>
	class App : public AppBase
	{
	public:
		App()
			: width_(0)
			, height_(0)
			, window_(new Window)
		{}

		virtual ~App()
		{}

		void Create(uint32 width, uint32 height, const char* title, bool wndmode = true)
		{
			window_->Create(width, height, title, wndmode);
			CreateInteral();
		}

		void CreateFullScreen(const char* title)
		{
			window_->CreateFullScreen(title);
			CreateInteral();
		}

		bool Loop() { return window_->Loop(); }

		void Destroy()
		{
			OnDestroy();

			ExitApp();

		}
	protected:
		void CreateInteral()
		{
			window_->SetApp(this);
			width_ = window_->GetWidth();
			height_ = window_->GetHeight();

			InitApp();

			OnCreate();
		}

		uint32 width_;
		uint32 height_;
		Window* window_;
	};
}
