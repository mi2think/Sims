/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   17:17
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_texture.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_texture
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	OGL Texture
*********************************************************************/
#ifndef __OGL_TEXTURE_H__
#define __OGL_TEXTURE_H__

#include "graphics/texture.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLTexture : public Texture
		{
		public:
			OGLTexture();
			OGLTexture(uint32 width, uint32 height, PixelFormat::Type format);
			OGLTexture(const string& path, PixelFormat::Type format);
			OGLTexture(const ImageRef& image);
			~OGLTexture();

			virtual void HWUpdateTexture(Recti* regions);
			virtual void HWBindTexture(uint32 textureUnit);
			virtual void HWDeleteTexture();
		private:
			GLuint tex_;
		};
	}
}

#endif
