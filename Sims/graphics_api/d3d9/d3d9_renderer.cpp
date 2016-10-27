/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:13
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_renderer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_renderer
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	D3D9 Renderer
*********************************************************************/
#include "d3d9_renderer.h"
#include "d3d9_common.h"
#include "graphics/texture.h"

namespace sims
{
	namespace d3d9
	{
		void D3D9Renderer::BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			DWORD flags = 0;
			if ((clearFlags & ClearFlags::Color) != 0)
				flags |= D3DCLEAR_TARGET;
			if ((clearFlags & ClearFlags::Depth) != 0)
				flags |= D3DCLEAR_ZBUFFER;
			if ((clearFlags & ClearFlags::Stencil) != 0)
				flags |= D3DCLEAR_STENCIL;

			CHECK_HR = g_pD3DD->Clear(0, 0, flags, color.value, depth, stencil);
			CHECK_HR = g_pD3DD->BeginScene();
		}

		void D3D9Renderer::EndFrame()
		{
			CHECK_HR = g_pD3DD->EndScene();
		}

		void D3D9Renderer::PresentFrame()
		{
			CHECK_HR = g_pD3DD->Present(0, 0, 0, 0);
		}

		void D3D9Renderer::UpdateTexture(Texture& texture, Recti* regions)
		{
			IDirect3DTexture9* id = (IDirect3DTexture9*)texture.GetRenderID();

			ImageRef image = texture.GetImage(0);
			uint32 bytesPerPixel = image->GetBytesPerPixel();

			// create texture
			if (!id)
			{
				D3DFORMAT format = ToD3DFormat(texture.GetFormat());
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((texture.GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				CHECK_HR = g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					texture.GetMipmapCount(),
					0,
					format,
					pool,
					&id,
					0);
				texture.SetRenderID(id);
			}

			// update texture
			IDirect3DTexture9* tex = id;

			// pool is D3DPOOL_DEFAULT. can't lock directly, 
			// create a system memory texture
			if ((texture.GetStorageFlags() & StorageFlags::HintDynamic) != 0)
			{
				D3DFORMAT format = ToD3DFormat(texture.GetFormat());
				CHECK_HR = g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					texture.GetMipmapCount(),
					0,
					format,
					D3DPOOL_SYSTEMMEM,
					&tex,
					0);
			}

			// update each surface by mipmap of image
			IDirect3DSurface9* surface = nullptr;
			for (uint32 i = 0; i < tex->GetLevelCount(); ++i)
			{
				CHECK_HR = tex->GetSurfaceLevel(i, &surface);

				image = texture.GetImage(i);
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

			if (tex != id)
			{
				CHECK_HR = g_pD3DD->UpdateTexture(tex, id);
				SAFE_RELEASE(tex);
			}
		}

		void D3D9Renderer::BindTexture(Texture& texture, uint32 textureUnit)
		{
			IDirect3DTexture9* tex = (IDirect3DTexture9*)texture.GetRenderID();
			ASSERT(tex != nullptr);

			CHECK_HR = g_pD3DD->SetTexture(textureUnit, tex);
			// filter
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_MAGFILTER, ToD3DTextureFilterType(texture.GetFilterMag()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_MINFILTER, ToD3DTextureFilterType(texture.GetFilterMin()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_MIPFILTER, ToD3DTextureFilterType(texture.GetFilterMip()));
			// wrap
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_ADDRESSU, ToD3DTextureAddress(texture.GetWrapS()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_ADDRESSV, ToD3DTextureAddress(texture.GetWrapT()));
			CHECK_HR = g_pD3DD->SetSamplerState(textureUnit, D3DSAMP_BORDERCOLOR, texture.GetBorderColor().value);
		}

		void D3D9Renderer::DeleteTexture(Texture& texture)
		{
			IDirect3DTexture9* tex = (IDirect3DTexture9*)texture.GetRenderID();
			ASSERT(tex != nullptr);
			SAFE_RELEASE(tex);
			texture.SetRenderID(0);
		}
	}
}