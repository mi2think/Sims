/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:26
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\renderer_caps_ogl.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	renderer_caps_ogl
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Renderer Caps ogl
*********************************************************************/
#include "renderer_caps_ogl.h"
#include "common_ogl.h"

namespace sims
{
	namespace ogl
	{
		RendererCaps g_RendererCaps;

		void RendererCaps::Dump()
		{
			LOG_INFO("CAPS: -----------------------");
			LOG_INFO("CAPS:  ogl renderer caps");
			LOG_INFO("CAPS:  Vendor: %s", vendor);
			LOG_INFO("CAPS:  Renderer: %s", renderer);
			LOG_INFO("CAPS:  Version: %s", version);
			LOG_INFO("CAPS:  GLSLVersion: %s", glslVersion);
		}

		void InitRendererCaps()
		{
			auto& caps = g_RendererCaps;
		
			// https://www.opengl.org/wiki/GLAPI/glGetString

			caps.vendor = (const char*)glGetString(GL_VENDOR);
			if (!caps.vendor)
				caps.vendor = "<undefined>";

			caps.renderer = (const char*)glGetString(GL_RENDER);
			if (!caps.renderer)
				caps.renderer = "<undefined>";

			caps.version = (const char*)glGetString(GL_VERSION);
			if (!caps.version)
				caps.version = "<undefined>";

			caps.glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
			if (!caps.glslVersion)
				caps.glslVersion = "<undefined>";
		}
	}
}
