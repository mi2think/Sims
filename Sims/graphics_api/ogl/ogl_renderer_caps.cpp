/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:26
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_renderer_caps.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_renderer_caps
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Renderer Caps ogl
*********************************************************************/
#include "ogl_renderer_caps.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		RendererCaps g_RendererCaps;

		void RendererCaps::Dump()
		{
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  ogl renderer caps");
			LOG_INFO("CAPS:  Vendor: %s", vendor);
			LOG_INFO("CAPS:  Renderer: %s", renderer);
			LOG_INFO("CAPS:  Version: %s", version);
			LOG_INFO("CAPS:  GLSLVersion: %s", glslVersion);
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  ogl extensions:");

			// extension is a space-separated list of tokens
			const char* p = extensions;
			for (const char* q = p; *q;)
			{
				while (*q != 0 && *q != ' ')
					++q;
				if (q != p)
				{
					LOG_INFO("CAPS:     %s", string(p, q - p).c_str());
				}
				// start on next token, if any
				p = *q ? q + 1 : q;
				q = p;
			}
		}

		void InitRendererCaps()
		{
			auto& caps = g_RendererCaps;
		
			// https://www.opengl.org/wiki/GLAPI/glGetString

			caps.vendor = (const char*)glGetString(GL_VENDOR);
			if (!caps.vendor)
				caps.vendor = "<undefined>";

			caps.renderer = (const char*)glGetString(GL_RENDERER);
			if (!caps.renderer)
				caps.renderer = "<undefined>";

			caps.version = (const char*)glGetString(GL_VERSION);
			if (!caps.version)
				caps.version = "<undefined>";

			caps.glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
			if (!caps.glslVersion)
				caps.glslVersion = "<undefined>";

			caps.extensions = (const char*)glGetString(GL_EXTENSIONS);
			if (!caps.extensions)
				caps.extensions = "no extensions";
		}

		bool HasExtension(const char* extension)
		{
			if (!g_RendererCaps.extensions)
				return false;

			// extension is a space-separated list of tokens
			const char* p = extension;
			for (const char* q = p; *q;)
			{
				while (*q != 0 && *q != ' ')
					++q;

				// if this match token
				if (q != p && strncmp(extension, p, q - p) == 0)
					return true;

				// start on next token, if any
				p = *q ? q + 1 : q;
			}

			return false;
		}
	}
}
