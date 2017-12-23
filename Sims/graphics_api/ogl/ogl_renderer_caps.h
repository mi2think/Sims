/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:17
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_renderer_caps.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_renderer_caps
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Renderer Caps ogl
*********************************************************************/
#ifndef __RENDERER_CAPS_OGL_H__
#define __RENDERER_CAPS_OGL_H__

#include "sims.h"

namespace sims
{
	namespace ogl
	{
		class OGLCapsInfo
		{
		public:
			static void InitCapsInfo();
			static void DumpCapsInfo();
			static bool HasExtension(const char* extension);

			static bool SupportVertexArrayObjects() { return bSupportVertexArrayObjects; }
		private:
			struct BaseCapsInfo
			{
				const char* vendor;
				const char* renderer;
				const char* version;
				const char* glslVersion;
				const char* extensions;
			};

			static BaseCapsInfo baseCapsInfo;
			static int32 majorVersion;
			static int32 minorVersion;

			static bool bSupportVertexArrayObjects;
		};
	}
}

#endif
