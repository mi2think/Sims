/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   18:05
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\renderer_caps.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_renderer_caps
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Renderer Caps
*********************************************************************/
#include "renderer_caps.h"
#include "common.h"

namespace sims
{
	namespace d3d9
	{
		RendererCaps g_RendererCaps;

		void RendererCaps::Dump()
		{
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  dx9 renderer caps");
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  Adapter info");
			LOG_INFO("CAPS:  Count: %d", adpCount);
			LOG_INFO("CAPS:  Dirver Desc: %s", adpDriverDesc.c_str());
			LOG_INFO("CAPS:  Dirver Version: %s", adpDriverVersion.c_str());
			LOG_INFO("CAPS: -------------------------------------");
			LOG_INFO("CAPS:  Vertex Shader Version: %d.%d", D3DSHADER_VERSION_MAJOR(VSVersion), D3DSHADER_VERSION_MINOR(VSVersion));
			LOG_INFO("CAPS:  Pixel Shader Version: %d.%d", D3DSHADER_VERSION_MAJOR(PSVersion), D3DSHADER_VERSION_MINOR(PSVersion));
			LOG_INFO("CAPS:  Vertex Shader Profile: %s", VSVersionProfile.c_str());
			LOG_INFO("CAPS:  Pixel Shader Profile: %s", PSVersionProfile.c_str());
		}

		void InitRendererCaps()
		{
			auto& caps = g_RendererCaps;

			D3DADAPTER_IDENTIFIER9 iden;
			g_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &iden);
			DWORD Product = HIWORD(iden.DriverVersion.HighPart);
			DWORD Version = LOWORD(iden.DriverVersion.HighPart);
			DWORD SubVersion = HIWORD(iden.DriverVersion.LowPart);
			DWORD Build = LOWORD(iden.DriverVersion.LowPart);

			caps.adpCount = g_pD3D->GetAdapterCount();
			caps.adpDriverDesc = str_format("%s %s", iden.Driver, iden.Description);
			caps.adpDriverVersion = str_format("%d.%d.%d.%d", Product, Version, SubVersion, Build);

			D3DCAPS9 dx9caps;
			g_pD3DD->GetDeviceCaps(&dx9caps);

			caps.VSVersion = dx9caps.VertexShaderVersion;
			caps.PSVersion = dx9caps.PixelShaderVersion;
			caps.VSVersionProfile = D3DXGetVertexShaderProfile(g_pD3DD);
			caps.PSVersionProfile = D3DXGetPixelShaderProfile(g_pD3DD);
		}
	}
}