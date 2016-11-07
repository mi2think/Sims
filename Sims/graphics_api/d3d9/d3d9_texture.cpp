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

namespace sims
{
	namespace d3d9
	{
		D3D9Texture::D3D9Texture()
			: Texture()
			, tex_(nullptr)
		{
		}

		D3D9Texture::D3D9Texture(uint32 width, uint32 height, PixelFormat::Type format, uint32 storgeFlags)
			: Texture(width, height, format, storgeFlags)
			, tex_(nullptr)
		{
		}

		D3D9Texture::D3D9Texture(const string& path, PixelFormat::Type format, uint32 storgeFlags)
			: Texture(path, format, storgeFlags)
			, tex_(nullptr)
		{
		}

		D3D9Texture::D3D9Texture(const ImageRef& image, uint32 storgeFlags)
			: Texture(image, storgeFlags)
			, tex_(nullptr)
		{
		}

		D3D9Texture::~D3D9Texture()
		{
			SAFE_RELEASE(tex_);
		}

		void D3D9Texture::HWUpdateTexture(Recti* regions)
		{
			ImageRef image = GetImage(0);
			uint32 bytesPerPixel = image->GetBytesPerPixel();

			// create texture
			if (!tex_)
			{
				D3DFORMAT format = ToD3DFormat(format_);
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((storageFlags_ & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				CHECK_HR = g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					mipmapCount_,
					0,
					format,
					pool,
					&tex_,
					0);
			}

			// update texture
			IDirect3DTexture9* tempTex = tex_;

			// pool is D3DPOOL_DEFAULT. can't lock directly, 
			// create a system memory texture
			if ((storageFlags_ & StorageFlags::HintDynamic) != 0)
			{
				D3DFORMAT format = ToD3DFormat(format_);
				CHECK_HR = g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					mipmapCount_,
					0,
					format,
					D3DPOOL_SYSTEMMEM,
					&tempTex,
					0);
			}

			// update each surface by mipmap of image
			IDirect3DSurface9* surface = nullptr;
			for (uint32 i = 0; i < tempTex->GetLevelCount(); ++i)
			{
				CHECK_HR = tempTex->GetSurfaceLevel(i, &surface);

				image = GetImage(i);
				uint32 width = image->GetWidth();
				uint32 height = image->GetHeight();
				const uint8* src = image->GetConstData();

				D3DLOCKED_RECT lockedRect;
				CHECK_HR = surface->LockRect(&lockedRect, (RECT*)&regions[i], D3DLOCK_NOOVERWRITE);
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

			if (tempTex != tex_)
			{
				CHECK_HR = g_pD3DD->UpdateTexture(tempTex, tex_);
				SAFE_RELEASE(tempTex);
			}
		}

		void D3D9Texture::HWBindTexture(uint32 textureUnit)
		{
			ASSERT(tex_ != nullptr);

			CHECK_HR = g_pD3DD->SetTexture(textureUnit, tex_);
			// filter
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_MAGFILTER, ToD3DTextureFilterType(samplerStatus_.GetFilterMag()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_MINFILTER, ToD3DTextureFilterType(samplerStatus_.GetFilterMin()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_MIPFILTER, ToD3DTextureFilterType(samplerStatus_.GetFilterMip()));
			// wrap
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_ADDRESSU, ToD3DTextureAddress(samplerStatus_.GetWrapS()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_ADDRESSV, ToD3DTextureAddress(samplerStatus_.GetWrapT()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_BORDERCOLOR, samplerStatus_.GetBorderColor().value);
		}

		void D3D9Texture::HWDeleteTexture()
		{
			SAFE_RELEASE(tex_);
		}
	}
}