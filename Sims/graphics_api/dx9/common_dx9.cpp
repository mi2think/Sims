/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:05
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\common_dx9.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	common_dx9
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Common def
*********************************************************************/
#include "common_dx9.h"
#include "renderer_dx9.h"
#include "core/log.h"

#include <DxErr.h>

namespace sims
{
	namespace dx9
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

		void PrintAdapterCount()
		{
			LOG_INFO("adapter count:%d\n", g_pD3D->GetAdapterCount());
		}

		void PrintAdapterIdentifier()
		{
			D3DADAPTER_IDENTIFIER9 iden;
			CHECK_HR = g_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &iden);
			LOG_INFO("Adapter info:\nDirver:%s\nDescription:%s", iden.Driver, iden.Description);
			DWORD Product = HIWORD(iden.DriverVersion.HighPart);
			DWORD Version = LOWORD(iden.DriverVersion.HighPart);
			DWORD SubVersion = HIWORD(iden.DriverVersion.LowPart);
			DWORD Build = LOWORD(iden.DriverVersion.LowPart);
			LOG_INFO("\t%d.%d.%d.%d\n", Product, Version, SubVersion, Build);
		}

		D3DFORMAT ToD3DFormat(PixelFormat format)
		{
			switch (format)
			{
			case PF_A8:
				return D3DFMT_A8;
			case PF_R8G8B8:
				return D3DFMT_R8G8B8;
			case PF_R8G8B8A8:
				return D3DFMT_A8B8G8R8; // need test
			case PF_A8R8G8B8:
				return D3DFMT_A8R8G8B8;
			default:
				ASSERT(false);
				break;
			}
			return D3DFMT_A8R8G8B8;
		}
	}
}