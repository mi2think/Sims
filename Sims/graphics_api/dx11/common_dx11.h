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

#pragma warning(push)
#pragma warning( disable : 4838 4005 )  // disable narrow-cast warning in xnamath

#include "sims.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"

#include <windows.h> // must forward include if include xnamath
#include <xnamath.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dx11.h>

namespace sims
{
	namespace dx11
	{
		XMGLOBALCONST XMVECTORF32 White   = { 1.0f, 1.0f, 1.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Black   = { 0.0f, 0.0f, 0.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Red     = { 1.0f, 0.0f, 0.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Green   = { 0.0f, 1.0f, 0.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
		XMGLOBALCONST XMVECTORF32 Silver  = { 0.75f, 0.75f, 0.75f, 1.0f };
		XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };

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

		// vertex define
		struct Vertex
		{
			Vector3f p;
			Vector4f c;
			Vertex() = default;
			Vertex(const Vector3f& _p, const Vector4f& _c)
				: p(_p)
				, c(_c)
			{}

			static uint32 GetDescNum()
			{
				return 2;
			}
			static D3D11_INPUT_ELEMENT_DESC* GetDesc()
			{
				static D3D11_INPUT_ELEMENT_DESC desc[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				return desc;
			}
		};

		struct VertexN
		{
			Vector3f p;
			Vector3f n;
			VertexN() = default;
			VertexN(const Vector3f& _p, const Vector3f& _n)
				: p(_p)
				, n(_n)
			{}

			static uint32 GetDescNum()
			{
				return 2;
			}
			static D3D11_INPUT_ELEMENT_DESC* GetDesc()
			{
				static D3D11_INPUT_ELEMENT_DESC desc[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				return desc;
			}
		};

		struct VertexUV
		{
			Vector3f p;
			Vector2f uv;
			VertexUV() = default;
			VertexUV(const Vector3f& _p, const Vector2f& _uv)
				: p(_p)
				, uv(_uv)
			{}

			static uint32 GetDescNum()
			{
				return 2;
			}
			static D3D11_INPUT_ELEMENT_DESC* GetDesc()
			{
				static D3D11_INPUT_ELEMENT_DESC desc[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				return desc;
			}
		};

		struct VertexUVN
		{
			Vector3f p;
			Vector3f n;
			Vector2f uv;
			VertexUVN() = default;
			VertexUVN(const Vector3f& _p, const Vector3f& _n, const Vector2f& _uv)
				: p(_p)
				, n(_n)
				, uv(_uv)
			{}

			static uint32 GetDescNum()
			{
				return 3;
			}
			static D3D11_INPUT_ELEMENT_DESC* GetDesc()
			{
				static D3D11_INPUT_ELEMENT_DESC desc[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				return desc;
			}
		};

		// traits offset of VertexXXX
		template <typename T> struct Offset
		{
			static const uint16 P  = 0;
			static const uint16 N  = 0;
			static const uint16 UV = 0;
		};
		template<> struct Offset<Vertex>
		{
			static const uint16 P = offsetof(Vertex, p);
			static const uint16 N = 0;
			static const uint16 UV = 0;
		};
		template<> struct Offset<VertexN>
		{
			static const uint16 P = offsetof(VertexN, p);
			static const uint16 N = offsetof(VertexN, n);
			static const uint16 UV = 0;
		};
		template<> struct Offset<VertexUV>
		{
			static const uint16 P = offsetof(VertexUV, p);
			static const uint16 N = 0;
			static const uint16 UV = offsetof(VertexUV, uv);
		};
		template<> struct Offset<VertexUVN>
		{
			static const uint16 P = offsetof(VertexUVN, p);
			static const uint16 N = offsetof(VertexUVN, n);
			static const uint16 UV = offsetof(VertexUVN, uv);
		};
	}
}

#pragma warning( pop )  
#endif
