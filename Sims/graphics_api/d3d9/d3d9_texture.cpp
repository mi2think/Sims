/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   16:59
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_texture.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_texture
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	D3D9 Texture
*********************************************************************/
#include "d3d9_texture.h"
#include "graphics/texture.h"

namespace sims
{
	namespace d3d9
	{
		D3DTextureResource::~D3DTextureResource()
		{
			ASSERT(! resource_);
		}

		D3DTextureResource::D3DTextureResource()
			: TextureResource()
			, resource_(nullptr)
		{}

		void D3DTextureResource::UpdateResource()
		{
			ImageRef image = texture_->GetImage(0);
			uint32 bytesPerPixel = image->GetBytesPerPixel();
			uint32 storageFlags = texture_->GetStorageFlags();

			// create resource
			if (!resource_)
			{
				D3DFORMAT format = ToD3DFormat(texture_->GetFormat());
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((storageFlags & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				CHECK_HR = g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					texture_->GetMipmapCount(),
					0,
					format,
					pool,
					&resource_,
					0);
			}

			// update texture
			IDirect3DTexture9* tempResource = resource_;

			// pool is D3DPOOL_DEFAULT. can't lock directly, 
			// create a system memory texture
			if ((storageFlags & StorageFlags::HintDynamic) != 0)
			{
				D3DFORMAT format = ToD3DFormat(texture_->GetFormat());
				CHECK_HR = g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					texture_->GetMipmapCount(),
					0,
					format,
					D3DPOOL_SYSTEMMEM,
					&resource_,
					0);
			}

			// update each surface by mipmap of image
			IDirect3DSurface9* surface = nullptr;
			for (uint32 i = 0; i < tempResource->GetLevelCount(); ++i)
			{
				CHECK_HR = tempResource->GetSurfaceLevel(i, &surface);

				image = texture_->GetImage(i);
				uint32 width = image->GetWidth();
				uint32 height = image->GetHeight();
				const char* src = image->GetConstData();

				D3DLOCKED_RECT lockedRect;
				CHECK_HR = surface->LockRect(&lockedRect, (RECT*)&regions_[i], D3DLOCK_NOOVERWRITE);
				uint8* dest = (uint8*)lockedRect.pBits;
				for (uint32 j = 0; j < height; ++j)
				{
					memcpy(dest, src, width * bytesPerPixel);
					src += width * bytesPerPixel;
					dest += lockedRect.Pitch;
				}
				CHECK_HR = surface->UnlockRect();

				SAFE_RELEASE(surface);
			}

			if (tempResource != resource_)
			{
				CHECK_HR = g_pD3DD->UpdateTexture(tempResource, resource_);
				SAFE_RELEASE(tempResource);
			}
		}

		void D3DTextureResource::BindResource()
		{
			ASSERT(resource_ != nullptr);

			const auto& samplerStatus = texture_->GetSamplerStatus();

			CHECK_HR = g_pD3DD->SetTexture(bindStage_, resource_);
			// filter
			CHECK_HR = g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_MAGFILTER, ToD3DTextureFilterType(samplerStatus.GetFilterMag()));
			CHECK_HR = g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_MINFILTER, ToD3DTextureFilterType(samplerStatus.GetFilterMin()));
			CHECK_HR = g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_MIPFILTER, ToD3DTextureFilterType(samplerStatus.GetFilterMip()));
			// wrap
			CHECK_HR = g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_ADDRESSU, ToD3DTextureAddress(samplerStatus.GetWrapS()));
			CHECK_HR = g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_ADDRESSV, ToD3DTextureAddress(samplerStatus.GetWrapT()));
			CHECK_HR = g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_BORDERCOLOR, samplerStatus.GetBorderColor().value);
		}

		void D3DTextureResource::ReleaseResource()
		{
			SAFE_RELEASE(resource_);
		}
	}
}