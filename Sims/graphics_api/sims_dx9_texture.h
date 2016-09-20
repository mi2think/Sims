/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   15:32
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_dx9_texture.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_dx9_texture
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	DX9 Texture
*********************************************************************/
#ifndef __SIMS_DX9_TEXTURE_H__
#define __SIMS_DX9_TEXTURE_H__

#include "core/texture.h"
#include "sims_dx9.h"

namespace sims
{
	namespace dx9
	{
		class DX9Texture : public Texture
		{
		public:
			DX9Texture();
			DX9Texture(uint32 width, uint32 height, PixelFormat format, uint32 usage, uint32 pool);
			DX9Texture(const string& path, uint32 usage, uint32 pool);
			DX9Texture(const ImageRef& image, uint32 usage, uint32 pool);

			uint32 GetUsage() const { return usage_; }
			uint32 GetPool() const { return pool_; }
			void SetUsage(uint32 usage) { usage_ = usage; }
			void SetPool(uint32 pool) { pool_ = pool; }

			virtual void Bind(int stage);
			virtual void Unbind(int stage);
			virtual void Destroy();
		private:
			uint32 usage_;
			uint32 pool_;
			IDirect3DTexture9* tex_;
		};
	}
}

#endif