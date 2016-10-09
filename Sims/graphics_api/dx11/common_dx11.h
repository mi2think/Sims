/********************************************************************
	created:	2016/10/09
	created:	9:10:2016   21:59
	filename: 	D:\Code\Sims\Sims\graphics_api\dx11\common_dx11.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx11
	file base:	common_dx11
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Common def
*********************************************************************/
#ifndef __COMMON_DX11_H__
#define __COMMON_DX11_H__

#include "sims.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3dx11.h>

namespace sims
{
	namespace dx11
	{
		// error checker
		class DXErrorChecker
		{
		public:
			DXErrorChecker(const char* file, uint32 line);
			~DXErrorChecker();

			DXErrorChecker& operator=(HRESULT hr);
		private:
			const char* file_;
			uint32 line_;
			HRESULT hr_;
		};

#define CHECK_HR DXErrorChecker(__FILE__, __LINE__)

		extern ID3D11Device* g_pD3DD;
		extern ID3D11DeviceContext* g_pD3DDC;
		extern IDXGISwapChain* g_pSwapChain;
		extern ID3D11Texture2D* g_pDepthStencilBuffer;
		extern ID3D11RenderTargetView* g_pRenderTargetView;
		extern ID3D11DepthStencilView* g_pDepthStencilView;

		ID3D11Device* SetupD3DDevice(HWND hwnd, int width, int height, bool wndmode);
		void ResizeDevice(int width, int height);
		void DestoryDirectX();
	}
}


#endif
