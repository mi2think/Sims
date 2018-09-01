/********************************************************************
	created:	2017/08/16
	created:	16:8:2017   11:40
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\glut_callbacks.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	glut_callbacks
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	GLUT Callbacks
*********************************************************************/
#pragma once

namespace sims
{
	namespace ogl
	{
		class GLUT_Callbacks
		{
		public:
			virtual ~GLUT_Callbacks() {}

			virtual void KeyboardFunc(unsigned char key, int x, int y) = 0;
			virtual void KeyboardSpecialFunc(int key, int x, int y) = 0;
			virtual void KeyboardUpFunc(unsigned char key, int x, int y) = 0;

			virtual void MouseFunc(int button, int state, int x, int y) = 0;
			virtual void MotionFunc(int x, int y) = 0;
			virtual void PassiveMotionFunc(int x, int y) = 0;

			virtual void DisplayFunc() = 0;
			virtual void IdleFunc() = 0;
		};
	}
}
