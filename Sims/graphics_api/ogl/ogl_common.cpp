/********************************************************************
	created:	2016/10/12
	created:	12:10:2016   21:38
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_common.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_common
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	OGL
*********************************************************************/
#include "ogl_common.h"
#include "core/log.h"

namespace sims
{
	namespace ogl
	{
		GLUT_Callbacks* g_callback = nullptr;

		static void KeyboardFunc(unsigned char key, int x, int y) { g_callback->KeyboardFunc(key, x, y);}
		static void KeyboardSpecialFunc(int key, int x, int y) {g_callback->KeyboardSpecialFunc(key, x, y);}
		static void KeyboardUpFunc(unsigned char key, int x, int y) { g_callback->KeyboardUpFunc(key, x, y); }

		static void MouseFunc(int button, int state, int x, int y) { g_callback->MouseFunc(button, state, x, y); }
		static void MotionFunc(int x, int y) { g_callback->MotionFunc(x, y); }
		static void PassiveMotionFunc(int x, int y) { g_callback->PassiveMotionFunc(x, y); }

		static void DisplayFunc() { g_callback->DisplayFunc(); }
		static void IdleFunc() { g_callback->IdleFunc(); }

		void glut_init()
		{
			static bool hasInit = false;
			if (hasInit)
				return;

			// hack for empty params
			int argc = 1;
			char* argv[] = { "dummy" };
			glutInit(&argc, argv);

			// init glut
			uint32 displayMode = GLUT_DOUBLE | GLUT_RGBA;
			displayMode |= GLUT_DEPTH;  // with depth
			displayMode |= GLUT_STENCIL;// with stencil
			glutInitDisplayMode(displayMode);
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);	
			
			// disable this for in test function allow VBO without wrap in VAO
			// http://gamedev.stackexchange.com/questions/28306/vbo-triangle-not-rendered-using-freeglut-opengl-3-x-not-understanding-the-weird
			//glutInitContextVersion(4, 3);

			hasInit = true;
		}

		void glut_create_window(uint32 width, uint32 height, const char* title, bool fullscreen)
		{
			if (fullscreen)
			{
				uint32 bpp = 32;
				glutGameModeString(str_format("%dx%d:%d@60", width, height, bpp));
				glutEnterGameMode();
			}
			else
			{
				glutInitWindowSize(width, height);
				// center window
				int screenW = glutGet(GLUT_SCREEN_WIDTH);
				int screenH = glutGet(GLUT_SCREEN_HEIGHT);
				glutInitWindowPosition((screenW - width) / 2, (screenH - height) / 2);
				glutCreateWindow(title);
			}
		}

		void glut_main_loop()
		{
			glutMainLoop();
		}

		void glut_bind_callbacks()
		{
			ASSERT(g_callback != nullptr);

			glutKeyboardFunc(KeyboardFunc);
			glutSpecialFunc(KeyboardSpecialFunc);
			glutKeyboardUpFunc(KeyboardUpFunc);

			glutMouseFunc(MouseFunc);
			glutMotionFunc(MotionFunc);
			glutPassiveMotionFunc(PassiveMotionFunc);

			glutDisplayFunc(DisplayFunc);
			glutIdleFunc(IdleFunc);
		}

		void gl_init_states(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
		}

		GLenum gl_check_error(const char* category)
		{
			GLenum err = glGetError();
			if (err != 0)
			{
				if (category != nullptr)
					LOG_ERROR("[%s] gl error:%d", category, err);
				else
					LOG_ERROR("gl error:%d", err);
			}
			return err;
		}
	}
}