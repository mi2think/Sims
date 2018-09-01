/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   17:26
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\renderer_caps.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	renderer_caps
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Renderer Caps ogl
*********************************************************************/
#include "renderer_caps.h"
#include "common.h"

namespace sims
{
	namespace ogl
	{
		OGLCapsInfo::BaseCapsInfo OGLCapsInfo::baseCapsInfo;
		int32 OGLCapsInfo::majorVersion = 0;
		int32 OGLCapsInfo::minorVersion = 0;
		bool OGLCapsInfo::bSupportVertexArrayObjects;

		void OGLCapsInfo::DumpCapsInfo()
		{
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  ogl renderer caps");
			LOG_INFO("CAPS:  Vendor: %s", baseCapsInfo.vendor);
			LOG_INFO("CAPS:  Renderer: %s", baseCapsInfo.renderer);
			LOG_INFO("CAPS:  Version: %s", baseCapsInfo.version);
			LOG_INFO("CAPS:  GLSLVersion: %s", baseCapsInfo.glslVersion);
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  ogl extensions:");

			// extension is a space-separated list of tokens
			const char* p = baseCapsInfo.extensions;
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

			LOG_INFO("CAPS:  major version: %d", majorVersion);
			LOG_INFO("CAPS:  minor version: %d", minorVersion);
		}

		void OGLCapsInfo::InitCapsInfo()
		{		
			// https://www.opengl.org/wiki/GLAPI/glGetString

			baseCapsInfo.vendor = (const char*)glGetString(GL_VENDOR);
			if (!baseCapsInfo.vendor)
				baseCapsInfo.vendor = "<undefined>";

			baseCapsInfo.renderer = (const char*)glGetString(GL_RENDERER);
			if (!baseCapsInfo.renderer)
				baseCapsInfo.renderer = "<undefined>";

			baseCapsInfo.version = (const char*)glGetString(GL_VERSION);
			if (!baseCapsInfo.version)
				baseCapsInfo.version = "<undefined>";

			baseCapsInfo.glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
			if (!baseCapsInfo.glslVersion)
				baseCapsInfo.glslVersion = "<undefined>";

			baseCapsInfo.extensions = (const char*)glGetString(GL_EXTENSIONS);
			if (!baseCapsInfo.extensions)
				baseCapsInfo.extensions = "no extensions";

			glGetIntegerv(GL_MAJOR_VERSION, (GLint*)&majorVersion);
			glGetIntegerv(GL_MINOR_VERSION, (GLint*)&minorVersion);

			// since 3.0
			bSupportVertexArrayObjects = (majorVersion >= 3) || HasExtension("GL_OES_vertex_array_object");
		}

		bool OGLCapsInfo::HasExtension(const char* extension)
		{
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
