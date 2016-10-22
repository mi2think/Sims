/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:05
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_common.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_common
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Common def
*********************************************************************/
#include "d3d9_common.h"
#include "d3d9_renderer.h"
#include "core/log.h"

#include <DxErr.h>

namespace sims
{
	namespace d3d9
	{
		// hack for dxerr.lib for it not compatible with vs2015
		int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

		DXErrorChecker::DXErrorChecker(const char* file, uint32 line)
			: file_(file)
			, line_(line)
			, hr_(D3D_OK)
		{}

		DXErrorChecker::~DXErrorChecker()
		{
			if (hr_ != D3D_OK)
			{
				const char* str = str_format("%s(%d):\n%s\n", file_, line_, DXGetErrorDescription(hr_));
				MessageBox(NULL, str, "error", MB_OK);
			}
		}

		DXErrorChecker& DXErrorChecker::operator=(HRESULT hr)
		{
			hr_ = hr;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
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

		const char* D3DFormat2String(D3DFORMAT format)
		{
			const char* str = "";
			switch (format)
			{
			case D3DFMT_R8G8B8:			str = "D3DFMT_R8G8B8";			break;
			case D3DFMT_A8R8G8B8:		str = "D3DFMT_A8R8G8B8";		break;
			case D3DFMT_X8R8G8B8:		str = "D3DFMT_X8R8G8B8";		break;
			case D3DFMT_R5G6B5:			str = "D3DFMT_R5G6B5";			break;
			case D3DFMT_X1R5G5B5:		str = "D3DFMT_X1R5G5B5";		break;
			case D3DFMT_A1R5G5B5:		str = "D3DFMT_A1R5G5B5";		break;
			case D3DFMT_A4R4G4B4:		str = "D3DFMT_A4R4G4B4";		break;
			case D3DFMT_R3G3B2:			str = "D3DFMT_R3G3B2";			break;
			case D3DFMT_A8:				str = "D3DFMT_A8";				break;
			case D3DFMT_A8R3G3B2:		str = "D3DFMT_A8R3G3B2";		break;
			case D3DFMT_X4R4G4B4:		str = "D3DFMT_X4R4G4B4";		break;
			case D3DFMT_A2B10G10R10:	str = "D3DFMT_A2B10G10R10";		break;
			case D3DFMT_A8B8G8R8:		str = "D3DFMT_A8B8G8R8";		break;
			case D3DFMT_X8B8G8R8:		str = "D3DFMT_X8B8G8R8";		break;
			case D3DFMT_G16R16:			str = "D3DFMT_G16R16";			break;
			case D3DFMT_A2R10G10B10:	str = "D3DFMT_A2R10G10B10";		break;
			case D3DFMT_A16B16G16R16:	str = "D3DFMT_A16B16G16R16";	break;
			default: str = "unknown format"; break;
			}
			return str;
		}

		void PrintDisplayMode()
		{
			D3DDISPLAYMODE dm;
			CHECK_HR = g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
			LOG_INFO("Width:%d\tHeight:%d\tRefreshRate:%d\tFormat:%s\n", dm.Width, dm.Height, dm.RefreshRate, D3DFormat2String(dm.Format));
		}

		D3DFORMAT ToD3DFormat(PixelFormat::Type format)
		{
			switch (format)
			{
			case PixelFormat::A8:
				return D3DFMT_A8;
			case PixelFormat::R8G8B8:
				return D3DFMT_R8G8B8;
			case PixelFormat::R8G8B8A8:
				return D3DFMT_A8B8G8R8; // need test
			case PixelFormat::A8R8G8B8:
				return D3DFMT_A8R8G8B8;
			default:
				ASSERT(false);
				break;
			}
			return D3DFMT_A8R8G8B8;
		}
	}
}