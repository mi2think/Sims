/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   16:05
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_dx9_texture.cpp
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_dx9_texture
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	DX9 Texture
*********************************************************************/
#include "sims_dx9_texture.h"
#include "core/image.h"

namespace sims
{
	namespace dx9
	{
		D3DFORMAT ToD3DFormat(PixelFormat format)
		{
			switch (format)
			{
			case PF_A8:
				return D3DFMT_A8;
			case PF_R8G8B8:
				return D3DFMT_R8G8B8;
			case PF_R8G8B8A8:
				return D3DFMT_A8B8G8R8;
			case PF_A8R8G8B8:
				return D3DFMT_A8R8G8B8;
			default:
				ASSERT(false);
				break;
			}
			return D3DFMT_A8R8G8B8;
		}

		D3DTEXTUREFILTERTYPE ToD3DTXF(TextureFilter filter)
		{
			switch (filter)
			{
			case TF_Point:
				return D3DTEXF_POINT;
			case TF_Linear:
				return D3DTEXF_LINEAR;
			case TF_Anisotropic:
				return D3DTEXF_ANISOTROPIC;
			default:
				ASSERT(false);
				break;
			}
			return D3DTEXF_LINEAR;
		}

		D3DTEXTUREADDRESS ToD3DTXAddress(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TW_Reapeat:
				return D3DTADDRESS_WRAP;
			case TW_Mirror:
				return D3DTADDRESS_MIRROR;
			case TW_Clamp:
				return D3DTADDRESS_CLAMP;
			case TW_Border:
				return D3DTADDRESS_BORDER;
			default:
				ASSERT(false);
				break;
			}
			return D3DTADDRESS_WRAP;
		}

		DX9Texture::DX9Texture()
			: Texture()
			, usage_(D3DUSAGE_WRITEONLY)
			, pool_(D3DPOOL_DEFAULT)
			, tex_(nullptr)
		{}

		DX9Texture::DX9Texture(uint32 width, uint32 height, PixelFormat format, uint32 usage, uint32 pool)
			: Texture(width, height, format)
			, usage_(usage)
			, pool_(pool)
			, tex_(nullptr)
		{}

		DX9Texture::DX9Texture(const string& path, uint32 usage, uint32 pool)
			: Texture(path)
			, usage_(usage)
			, pool_(pool)
			, tex_(nullptr)
		{}

		DX9Texture::DX9Texture(const ImageRef& image, uint32 usage, uint32 pool)
			: Texture(image)
			, usage_(usage)
			, pool_(pool)
			, tex_(nullptr)
		{}

		void DX9Texture::Bind(int stage)
		{
			if (!tex_)
			{
				CHECK_HR = g_pD3DD->CreateTexture(width_,
					height_,
					1,
					usage_,
					ToD3DFormat(format_),
					(D3DPOOL)pool_,
					&tex_,
					nullptr);

				ImageRef image = GetImage(0);
				ASSERT(image->Valid());
				uint32 bytesPerPixel = image->GetBytesPerPixel();
				uint8* src = image->GetData();

				IDirect3DTexture9* updateTex = tex_;
				if (pool_ == D3DPOOL_DEFAULT)
				{
					CHECK_HR = g_pD3DD->CreateTexture(width_,
						height_,
						1,
						0,
						ToD3DFormat(format_),
						D3DPOOL_SYSTEMMEM,
						&updateTex,
						0);
				}

				D3DLOCKED_RECT lockedRect{ 0 };
				CHECK_HR = updateTex->LockRect(0, &lockedRect, nullptr, 0);
				uint8* dest = (uint8*)lockedRect.pBits;
				for (uint32 i = 0; i < height_; ++i)
				{
					memcpy(dest, src, width_ * bytesPerPixel);
					src += width_ * bytesPerPixel;
					dest += lockedRect.Pitch;
				}
				CHECK_HR = updateTex->UnlockRect(0);

				if (pool_ == D3DPOOL_DEFAULT)
				{
					CHECK_HR = g_pD3DD->UpdateTexture(updateTex, tex_);
					SAFE_RELEASE(updateTex);
				}
			}

			CHECK_HR = g_pD3DD->SetSamplerState(0, D3DSAMP_MAGFILTER, ToD3DTXF(filterMag_));
			CHECK_HR = g_pD3DD->SetSamplerState(0, D3DSAMP_MINFILTER, ToD3DTXF(filterMin_));
			CHECK_HR = g_pD3DD->SetSamplerState(0, D3DSAMP_MIPFILTER, ToD3DTXF(filterMip_));
			
			CHECK_HR = g_pD3DD->SetSamplerState(0, D3DSAMP_ADDRESSU, ToD3DTXAddress(wrapS_));
			CHECK_HR = g_pD3DD->SetSamplerState(0, D3DSAMP_ADDRESSV, ToD3DTXAddress(wrapT_));
			CHECK_HR = g_pD3DD->SetSamplerState(0, D3DSAMP_BORDERCOLOR, borderColor_.value);

			CHECK_HR = g_pD3DD->SetTexture(stage, tex_);
		}

		void DX9Texture::Unbind(int stage)
		{
			CHECK_HR = g_pD3DD->SetTexture(stage, nullptr);
		}

		void DX9Texture::Destroy()
		{
			SAFE_RELEASE(tex_);
		}
	}
}