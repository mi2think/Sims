/********************************************************************
	created:	2016/10/12
	created:	12:10:2016   20:41
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\window_ogl.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	window_ogl
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Window OGL
*********************************************************************/
#include "window_ogl.h"
#include "core/log.h"

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
			LOG_INFO("ogl version:%s", glGetString(GL_VERSION));

			gl_init_states(0.0f, 0.0f, 0.0f, 0.0f);
		}

		void Window::CreateFullScreen(const char* title)
		{
			glut_init();

			int screenW = glutGet(GLUT_SCREEN_WIDTH);
			int screenH = glutGet(GLUT_SCREEN_HEIGHT);
			Create(screenW, screenH, title, false);
		}

		void Window::SetApp(DemoAppBase* app)
		{
			app_ = app;
		}

		bool Window::Loop()
		{
			g_callback = this;
			glut_bind_callbacks();
			glut_main_loop();
		}

		void Window::KeyboardFunc(unsigned char key, int x, int y)
		{

		}
	}
}