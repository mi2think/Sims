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

#include "graphics/render_resource.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9TextureResource : public TextureResource
		{
		public:
			D3D9TextureResource();
			~D3D9TextureResource();

			virtual void UpdateResource();
			virtual void BindResource();
			virtual void ReleaseResource();
		private:
			void InternalReleaseResource();

			IDirect3DTexture9* resource_;
		};
	}
}


#endif
