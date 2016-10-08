/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:13
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\renderer_dx9.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	renderer_dx9
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Renderer dx9
*********************************************************************/
#include "renderer_dx9.h"
#include "common_dx9.h"
#include "core/log.h"
#include "core/image.h"
#include "core/texture.h"

namespace sims
{
	namespace dx9
	{
		void DX9Renderer::BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			DWORD flags = 0;
			if ((clearFlags & CF_Color) != 0)
				flags |= D3DCLEAR_TARGET;
			if ((clearFlags & CF_Depth) != 0)
				flags |= D3DCLEAR_ZBUFFER;
			if ((clearFlags & CF_Stencil) != 0)
				flags |= D3DCLEAR_STENCIL;

			dx9::CHECK_HR = dx9::g_pD3DD->Clear(0, 0, flags, color.value, depth, stencil);
		}

		void DX9Renderer::EndFrame()
		{
			dx9::CHECK_HR = dx9::g_pD3DD->EndScene();
		}

		void DX9Renderer::PresentFrame()
		{
			dx9::CHECK_HR = dx9::g_pD3DD->Present(0, 0, 0, 0);
		}

		void DX9Renderer::UpdateTexture(Texture& texture, Recti* regions)
		{
			IDirect3DTexture9* id = (IDirect3DTexture9*)texture.GetRenderID();

			ImageRef image = texture.GetImage(0);
			uint32 bytesPerPixel = image->GetBytesPerPixel();

			// create texture
			if (!id)
			{
				D3DFORMAT format = ToD3DFormat(texture.GetFormat());
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((texture.GetStorageFlags() & Texture::SF_HintDynamic) != 0)
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
			if ((texture.GetStorageFlags() & Texture::SF_HintDynamic) != 0)
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

		void DX9Renderer::DeleteTexture(RenderID id)
		{
			IDirect3DTexture9* tex = (IDirect3DTexture9*)id;
			SAFE_RELEASE(tex);
		}
	}
}