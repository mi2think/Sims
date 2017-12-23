/********************************************************************
	created:	2016/10/12
	created:	12:10:2016   20:41
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_window.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_window
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Window OGL
*********************************************************************/
#include "ogl_window.h"
#include "ogl_common.h"
#include "ogl_renderer_caps.h"
#include "core/log.h"
#include "core/key_event.h"
#include "core/mouse_event.h"
#include "core/input_state.h"

namespace sims
{
	namespace ogl
	{
		Window::Window()
			: app_(nullptr)
		{
		}

		Window::~Window()
		{
		}

		void Window::Create(uint32 width, uint32 height, const char* title, bool wndmode)
		{
			glut_init();
			glut_create_window(width, height, title, !wndmode);

			width_ = width;
			height_ = height;

			// must be done after glut is initialized
			GLenum res = glewInit();
			ASSERT(res == GLEW_OK);
			
			// ogl caps
			OGLCapsInfo::InitCapsInfo();
			OGLCapsInfo::DumpCapsInfo();

			gl_init_states(0.0f, 0.0f, 0.0f, 0.0f);
		}

		void Window::CreateFullScreen(const char* title)
		{
			glut_init();

			int screenW = glutGet(GLUT_SCREEN_WIDTH);
			int screenH = glutGet(GLUT_SCREEN_HEIGHT);
			Create(screenW, screenH, title, false);
		}

		void Window::SetApp(AppBase* app)
		{
			app_ = app;
		}

		bool Window::Loop()
		{
			// bind call backs
			g_callback = this;
			glut_bind_callbacks();

			// enter loop
			glut_main_loop();

			// clean and exit
			g_callback = nullptr;
			app_->Destroy();
			return true;
		}

		void Window::KeyboardFunc(unsigned char key, int, int)
		{
			KeyPressEvent event(key);
			app_->OnEvent(event);
			InputState::OnKeyPress(event);
		}

		void Window::KeyboardSpecialFunc(int key, int, int)
		{
			KeyPressEvent event(key);
			app_->OnEvent(event);
			InputState::OnKeyPress(event);
		}

		void Window::KeyboardUpFunc(unsigned char key, int, int)
		{
			KeyReleaseEvent event(key);
			app_->OnEvent(event);
			InputState::OnKeyRelease(event);
		}

		void Window::MouseFunc(int button, int state, int x, int y)
		{
			MouseButtonType btn = MOUSE_BUTTON_COUNT;

			if (button == GLUT_LEFT_BUTTON)
				btn = MOUSE_LBUTTON;
			else if (button == GLUT_RIGHT_BUTTON)
				btn = MOUSE_RBUTTON;
			else if (button == GLUT_MIDDLE_BUTTON)
				btn = MOUSE_MBUTTON;

			if (btn < MOUSE_BUTTON_COUNT)
			{
				if (state == GLUT_UP)
				{
					MouseReleaseEvent event(x, y, btn);
					app_->OnEvent(event);
					InputState::OnMouseRelease(event);
				}
				else if (state == GLUT_DOWN)
				{
					MousePressEvent event(x, y, btn);
					app_->OnEvent(event);
					InputState::OnMousePress(event);
				}
			}
		}

		void Window::MotionFunc(int x, int y)
		{
			// drag with mouse
			MouseMoveEvent event(x, y);
			app_->OnEvent(event);
		}

		void Window::PassiveMotionFunc(int x, int y)
		{
			// just move
			MouseMoveEvent event(x, y);
			app_->OnEvent(event);
		}

		void Window::DisplayFunc()
		{
			float ts = clamp_t(timer_.Tick(), 0.0f, 1.0f);
			Timestep timestep(ts);

			app_->OnUpdate(timestep);
			app_->OnRender(timestep);

			timer_.Reset();
		}

		void Window::IdleFunc()
		{
			// also call display
			DisplayFunc();
		}
	}
}