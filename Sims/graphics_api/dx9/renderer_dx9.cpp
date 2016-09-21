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
#include "core/log.h"
#include "common_dx9.h"

namespace sims
{
	namespace dx9
	{
		IDirect3D9* g_pD3D = nullptr;
		IDirect3DDevice9* g_pD3DD = nullptr;

		IDirect3D9* SetupD3D()
		{
			g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
			if (!g_pD3D)
			{
				LOG_ERROR("create d3d9 failed!\n");
			}
			return g_pD3D;
		}

		IDirect3DDevice9* SetupD3DDevice(HWND hwnd, int width, int height, bool wndmode)
		{
			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));
			d3dpp.Windowed = wndmode ? TRUE : FALSE;
			d3dpp.hDeviceWindow = hwnd;
			d3dpp.BackBufferWidth = width;
			d3dpp.BackBufferHeight = height;
			d3dpp.BackBufferCount = 1;
			d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
			d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
			d3dpp.MultiSampleQuality = 0;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

			CHECK_HR = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DD);
			return g_pD3DD;
		}

		void DestoryDirectX()
		{
			SAFE_RELEASE(g_pD3DD);
			SAFE_RELEASE(g_pD3D);
		}
	}
}