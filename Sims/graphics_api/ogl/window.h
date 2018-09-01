/********************************************************************
	created:	2016/10/12
	created:	12:10:2016   20:40
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\window.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	window
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Window OGL
*********************************************************************/
#ifndef __WINDOW_OGL_H__
#define __WINDOW_OGL_H__

#include "sims.h"
#include "core/app.h"
#include "core/timer.h"
#include "glut_callbacks.h"

namespace sims
{
	namespace ogl
	{
		class Window : public GLUT_Callbacks
		{
		public:
			Window();
			~Window();

			void Create(uint32 width, uint32 height, const char* title, bool wndmode);
			void CreateFullScreen(const char* title);
			bool Loop();

			uint32 GetWidth() const { return width_; }
			uint32 GetHeight() const { return height_; }

			void SetApp(AppBase* app);

			// callbacks
			virtual void KeyboardFunc(unsigned char key, int, int);
			virtual void KeyboardSpecialFunc(int key, int, int);
			virtual void KeyboardUpFunc(unsigned char key, int, int);

			virtual void MouseFunc(int button, int state, int x, int y);
			virtual void MotionFunc(int x, int y);
			virtual void PassiveMotionFunc(int x, int y);

			virtual void DisplayFunc();
			virtual void IdleFunc();
		private:
			uint32 width_;
			uint32 height_;
			bool wndmode_;
			AppBase* app_;
			Timer timer_;
		};
	}
}

#endif
