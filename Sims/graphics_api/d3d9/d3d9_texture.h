/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   16:50
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_texture.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_texture
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	D3D9 Texture
*********************************************************************/
#ifndef __D3D9_TEXTURE_H__
#define __D3D9_TEXTURE_H__

#include "graphics/texture.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9Texture : public Texture
		{
		public:
			D3D9Texture();
			D3D9Texture(uint32 width, uint32 height, PixelFormat::Type format, uint32 storgeFlags);
			D3D9Texture(const string& path, PixelFormat::Type format, uint32 storgeFlags);
			D3D9Texture(const ImageRef& image, uint32 storgeFlags);
			~D3D9Texture();

			virtual void HWUpdateTexture(Recti* regions);
			virtual void HWBindTexture(uint32 textureUnit);
			virtual void HWDeleteTexture();
		private:
			IDirect3DTexture9* tex_;
		};
	}
}


#endif
