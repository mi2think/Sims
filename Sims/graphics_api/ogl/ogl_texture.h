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

#include "graphics/render_resource.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLTextureResource : public TextureResource
		{
		public:
			OGLTextureResource();
			~OGLTextureResource();

			virtual void UpdateResource();
			virtual void BindResource();
			virtual void ReleaseResource();
		private:
			GLuint resource_;
		};
	}
}

#endif
