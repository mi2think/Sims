/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   23:02
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_dx9.cpp
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_dx9
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	DX9
*********************************************************************/
#include "sims_dx9.h"
#include "core/key_event.h"
#include "core/mouse_event.h"
#include "core/input_state.h"

#include <windowsx.h>	// for GET_X_LPARAM and GET_Y_LPARAM

namespace sims
{
	namespace dx9
	{
		IDirect3D9* g_pD3D = nullptr;
		IDirect3DDevice9* g_pD3DD = nullptr;

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

		void ShowDisplayMode()
		{
			D3DDISPLAYMODE dm;
			CHECK_HR = g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
			LOG_INFO("Width:%d\tHeight:%d\tRefreshRate:%d\tFormat:%s\n", dm.Width, dm.Height, dm.RefreshRate, D3DFormat2String(dm.Format));
		}

		void ShowAdapterCount()
		{
			LOG_INFO("adapter count:%d\n", g_pD3D->GetAdapterCount());
		}

		void ShowAdapterIdentifier()
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

		//////////////////////////////////////////////////////////////////////////
		void CheckWin32Error(const char* file, int line)
		{
			DWORD error = GetLastError();
			if (error == 0)
				return;

			char buf[128] = { 0 };
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, error,
				LANG_SYSTEM_DEFAULT, buf, sizeof(buf), nullptr);
			const char* str = str_format("%s(%d):\n%s\n", file, line, buf);
			MessageBox(nullptr, str, "error", MB_OK);
		}
#define CHECK_WIN32_ERROR()  CheckWin32Error(__FILE__, __LINE__)

		HWND g_hwnd = NULL;
		Window* g_Window = nullptr;
		static int s_clickCount = 0;
		IDirect3DVertexBuffer9* g_pVB = nullptr;
		IDirect3DTexture9* g_pTex = nullptr;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (g_Window && g_hwnd == hWnd)
			{
				g_Window->MsgProc(hWnd, uMsg, wParam, lParam);
			}
			else
			{
				if (GetCapture() != g_hwnd && s_clickCount > 0)
					s_clickCount = 0;
			}

			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		Window::Window()
			: width_(0)
			, height_(0)
			, wndmode_(true)
			, app_(nullptr)
		{
		}

		Window::~Window()
		{
			g_Window = nullptr;
		}

		void Window::SetApp(DemoAppBase* app)
		{
			app_ = app;
		}

		HWND Window::GetHandle() const
		{
			return g_hwnd;
		}

		void Window::Create(uint32 width, uint32 height, const char* title, bool wndmode)
		{
			WNDCLASS wc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hbrBackground = NULL;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = NULL;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpfnWndProc = WndProc;
			wc.lpszClassName = "DEMO APP";
			wc.lpszMenuName = NULL;
			wc.style = CS_HREDRAW | CS_VREDRAW;

			if (!RegisterClass(&wc))
			{
				CHECK_WIN32_ERROR();
				return;
			}

			uint32 w = width;
			uint32 h = height;
			if (wndmode)
			{
				RECT rect = { 0, 0, (LONG)width, (LONG)height };
				AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 0);
				w = rect.right - rect.left;
				h = rect.bottom - rect.top;
			}

			g_hwnd = CreateWindow("DEMO APP", title, (wndmode ? (WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU) : (WS_POPUP | WS_VISIBLE)),
				(GetSystemMetrics(SM_CXSCREEN) - w) / 2, (GetSystemMetrics(SM_CYSCREEN) - h) / 2, w, h, NULL, NULL, wc.hInstance, NULL);
			if (!g_hwnd)
			{
				CHECK_WIN32_ERROR();
				return;
			}

			g_Window = this;

			width_ = width;
			height_ = height;
			wndmode_ = wndmode;

			ShowWindow(g_hwnd, SW_SHOW);
			UpdateWindow(g_hwnd);

			SetupDX();
		}

		void Window::CreateFullScreen(const char* title)
		{
			int cx_screen = GetSystemMetrics(SM_CXSCREEN);
			int cy_screen = GetSystemMetrics(SM_CYSCREEN);

			Create(cx_screen, cy_screen, title, false);
		}

		bool Window::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				KeyPressEvent event(wParam);
				app_->OnEvent(event);
				InputState::OnKeyPress(event);
				if (event.GetKey() == KEY_ESCAPE)
					SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				KeyReleaseEvent event(wParam);
				app_->OnEvent(event);
				InputState::OnKeyRelease(event);
			}
			break;
			case WM_MOUSEMOVE:
			{
				MouseMoveEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				app_->OnEvent(event);
			}
			break;
			case WM_LBUTTONDOWN:
			{
				SetCapture(g_hwnd);
				++s_clickCount;
				MousePressEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_LBUTTON);
				InputState::OnMousePress(event);
				app_->OnEvent(event);
			}
			break;
			case WM_LBUTTONUP:
			{
				if (--s_clickCount < 1)
				{
					s_clickCount = 0;
					ReleaseCapture();
				}
				MouseReleaseEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_LBUTTON);
				InputState::OnMouseRelease(event);
				app_->OnEvent(event);
			}
			break;
			case WM_RBUTTONDOWN:
			{
				SetCapture(g_hwnd);
				++s_clickCount;
				MousePressEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_RBUTTON);
				InputState::OnMousePress(event);
				app_->OnEvent(event);
			}
			break;
			case WM_RBUTTONUP:
			{
				if (--s_clickCount < 1)
				{
					s_clickCount = 0;
					ReleaseCapture();
				}
				MouseReleaseEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_RBUTTON);
				InputState::OnMouseRelease(event);
				app_->OnEvent(event);
			}
			break;
			case WM_MBUTTONDOWN:
			{
				SetCapture(g_hwnd);
				++s_clickCount;
				MousePressEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_MBUTTON);
				InputState::OnMousePress(event);
				app_->OnEvent(event);
			}
			break;
			case WM_MBUTTONUP:
			{
				if (--s_clickCount < 1)
				{
					s_clickCount = 0;
					ReleaseCapture();
				}
				MouseReleaseEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_MBUTTON);
				InputState::OnMouseRelease(event);
				app_->OnEvent(event);
			}
			break;
			case WM_MOUSEWHEEL:
			{
				MouseWheelEvent event(GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
				app_->OnEvent(event);
			}
			break;
			}
			return true;
		}

		void Window::SetupDX()
		{
			SetupD3D();
			SetupD3DDevice(g_hwnd, width_, height_, wndmode_);
		}

		bool Window::Loop()
		{
			MSG msg;
			while (true)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
						break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					Timestep timestep(timer_.Tick());

					app_->OnUpdate(timestep);
					app_->OnRender(timestep);

					timer_.Reset();
				}
			}

			app_->OnDestroy();

			DestoryDX();

			return false;
		}


		void Window::DestoryDX()
		{
			SAFE_RELEASE(g_pVB);
			SAFE_RELEASE(g_pTex);
			DestoryDirectX();
		}
	}
}