/********************************************************************
	created:	2016/10/09
	created:	9:10:2016   22:27
	filename: 	D:\Code\Sims\Sims\graphics_api\dx11\d3d11_common.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\dx11
	file base:	d3d11_common
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Common def
*********************************************************************/
#include "d3d11_common.h"
#include "core/log.h"

#include <DxErr.h>

namespace sims
{
	namespace d3d11
	{
		// hack for dxerr.lib for it not compatible with vs2015
		int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

		DXErrorChecker::DXErrorChecker(const char* file, uint32 line)
			: file_(file)
			, line_(line)
			, hr_(0)
		{}

		DXErrorChecker::~DXErrorChecker()
		{
			if (FAILED(hr_))
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
		ID3D11Device* g_pD3DD = nullptr;
		ID3D11DeviceContext* g_pD3DDC = nullptr;
		IDXGISwapChain* g_pSwapChain = nullptr;
		ID3D11Texture2D* g_pDepthStencilBuffer = nullptr;
		ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
		ID3D11DepthStencilView* g_pDepthStencilView = nullptr;

		ID3D11Device* SetupD3DDevice(HWND hwnd, int width, int height, bool wndmode)
		{
			D3D_FEATURE_LEVEL featureLevel;
			CHECK_HR = D3D11CreateDevice(nullptr, // default adapter
				D3D_DRIVER_TYPE_HARDWARE,
				0, // no software device
#ifdef _DEBUG
				D3D11_CREATE_DEVICE_DEBUG,
#else
				0,
#endif	
				nullptr, // choose greatest feature level supported
				0,
				D3D11_SDK_VERSION,
				&g_pD3DD,
				&featureLevel,
				&g_pD3DDC);
			if (featureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				LOG_ERROR("d3d feature level 11 unsupported!");
				return nullptr;
			}

			// swap chain desc
			DXGI_SWAP_CHAIN_DESC desc;
			desc.BufferDesc.Width = width;
			desc.BufferDesc.Height = height;
			desc.BufferDesc.RefreshRate.Numerator = 60;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.SampleDesc.Count = 1; // no MSAA
			desc.SampleDesc.Quality = 0;
			desc.BufferCount = 1;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = hwnd;
			desc.Windowed = wndmode;
			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			desc.Flags = 0;

			IDXGIDevice* dxGIDevice = nullptr;
			IDXGIAdapter* dxGIAdapter = nullptr;
			IDXGIFactory* dxGIFactory = nullptr;
			CHECK_HR = g_pD3DD->QueryInterface(IID_IDXGIDevice, (void**)&dxGIDevice);
			CHECK_HR = dxGIDevice->GetParent(IID_IDXGIAdapter, (void**)&dxGIAdapter);
			CHECK_HR = dxGIAdapter->GetParent(IID_IDXGIFactory, (void**)&dxGIFactory);
			CHECK_HR = dxGIFactory->CreateSwapChain(g_pD3DD,
				&desc,
				&g_pSwapChain);
			SAFE_RELEASE(dxGIDevice);
			SAFE_RELEASE(dxGIAdapter);
			SAFE_RELEASE(dxGIFactory);

			ResizeDevice(width, height);

			return g_pD3DD;
		}

		void ResizeDevice(int width, int height)
		{
			SAFE_RELEASE(g_pRenderTargetView);
			SAFE_RELEASE(g_pDepthStencilView);
			SAFE_RELEASE(g_pDepthStencilBuffer);

			// resize swap chain, and create render target view
			CHECK_HR = g_pSwapChain->ResizeBuffers(1,
				width,
				height,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				0);
			ID3D11Texture2D* backBuffer = nullptr;
			CHECK_HR = g_pSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);
			CHECK_HR = g_pD3DD->CreateRenderTargetView(backBuffer, nullptr, &g_pRenderTargetView);

			// create depth/stencil buffer and view
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			CHECK_HR = g_pD3DD->CreateTexture2D(&desc, nullptr, &g_pDepthStencilBuffer);
			CHECK_HR = g_pD3DD->CreateDepthStencilView(g_pDepthStencilBuffer, nullptr, &g_pDepthStencilView);

			// bind render target and depth/stencil view to pipeline
			g_pD3DDC->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

			// set viewport
			D3D11_VIEWPORT vp;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			vp.Width = (float)width;
			vp.Height = (float)height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			g_pD3DDC->RSSetViewports(1, &vp);
		}

		void DestoryDirectX()
		{
			SAFE_RELEASE(g_pRenderTargetView);
			SAFE_RELEASE(g_pDepthStencilView);
			SAFE_RELEASE(g_pDepthStencilBuffer);
			SAFE_RELEASE(g_pSwapChain);
			SAFE_RELEASE(g_pD3DDC);
			SAFE_RELEASE(g_pD3DD);
		}
	}
}