/********************************************************************
	created:	2016/10/09
	created:	9:10:2016   21:55
	filename: 	D:\Code\Sims\Sims\graphics_api\dx11\window_dx11.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx11
	file base:	window_dx11
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Window dx9
*********************************************************************/
#ifndef __WINDOW_DX11_H__
#define __WINDOW_DX11_H__

#include "sims.h"
#include "common_dx11.h"
#include "platform/windows/windows_window_base.h"

namespace sims
{
	namespace dx11
	{
		class Window : public WindowBase
		{
		public:
			Window()
				: pD3DD_(nullptr)
				, pD3DDC_(nullptr)
				, pSwapChain_(nullptr)
				, pDepthStencilBuffer_(nullptr)
				, pRenderTargetView_(nullptr)
				, pDepthStencilView_(nullptr)
			{}

			virtual void OnCreate()
			{
				SetupD3DDevice(GetHandle(), width_, height_, wndmode_);

				pD3DD_ = g_pD3DD;
				pD3DDC_ = g_pD3DDC;
				pSwapChain_ = g_pSwapChain;
				pDepthStencilBuffer_ = g_pDepthStencilBuffer;
				pRenderTargetView_ = g_pRenderTargetView;
				pDepthStencilView_ = g_pDepthStencilView;
			}

			virtual void OnDestroy()
			{
				DestoryDirectX();

				pD3DD_ = nullptr;
				pD3DDC_ = nullptr;
				pSwapChain_ = nullptr;
				pDepthStencilBuffer_ = nullptr;
				pRenderTargetView_ = nullptr;
				pDepthStencilView_ = nullptr;
			}
		protected:
			ID3D11Device* pD3DD_;
			ID3D11DeviceContext* pD3DDC_;
			IDXGISwapChain* pSwapChain_;
			ID3D11Texture2D* pDepthStencilBuffer_;
			ID3D11RenderTargetView* pRenderTargetView_;
			ID3D11DepthStencilView* pDepthStencilView_;
		};
	}
}

#endif