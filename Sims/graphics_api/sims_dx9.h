/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   22:25
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_dx9
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	DX9
*********************************************************************/
#ifndef __SIMS_DX9_H__
#define __SIMS_DX9_H__

#include "sims.h"
#include "core/timer.h"
#include "utils/demo_app.h"

#include <windows.h>
#include "DirectX/Include/d3d9.h"
#include "DirectX/Include/d3dx9.h"
#include "DirectX/Include/DxErr.h"

namespace sims
{
	namespace dx9
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

		extern IDirect3D9* g_pD3D;
		extern IDirect3DDevice9* g_pD3DD;

		// setup
		IDirect3D9* SetupD3D();
		IDirect3DDevice9* SetupD3DDevice(HWND hwnd, int width, int height, bool wndmode);
		void DestoryDirectX();
		// format
		const char* D3DFormat2String(D3DFORMAT format);
		// default display mode
		void ShowDisplayMode();
		void ShowAdapterCount();
		// default identifier
		void ShowAdapterIdentifier();

		//////////////////////////////////////////////////////////////////////////
		// dx9 window

		class Window
		{
		public:
			Window();
			~Window();

			void Create(uint32 width, uint32 height, const char* title, bool wndmode);
			void CreateFullScreen(const char* title);
			bool MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			bool Loop();

			uint32 GetWidth() const { return width_; }
			uint32 GetHeight() const { return height_; }
			HWND GetHandle() const;

			void SetApp(DemoAppBase* app);
			void SetupDX();
			void DestoryDX();
		private:
			uint32 width_;
			uint32 height_;
			bool wndmode_;
			DemoAppBase* app_;
			Timer timer_;
		};

	}
}


#endif
