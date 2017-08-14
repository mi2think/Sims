/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:49
	filename: 	D:\Code\Sims\Sims\platform\windows\windows_window_base.cpp
	file path:	D:\Code\Sims\Sims\platform\windows
	file base:	windows_window_base
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Window Base
*********************************************************************/
#include "windows_window_base.h"
#include "windows_platform.h"
#include "core/key_event.h"
#include "core/mouse_event.h"
#include "core/input_state.h"

#include <windows.h>

#include <windowsx.h>	// for GET_X_LPARAM and GET_Y_LPARAM

namespace sims
{
	HWND g_hwnd = NULL;
	WindowBase* g_Window = nullptr;
	static int s_clickCount = 0;

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

	WindowBase::WindowBase()
		: width_(0)
		, height_(0)
		, wndmode_(true)
		, app_(nullptr)
	{
	}

	WindowBase::~WindowBase()
	{
		g_Window = nullptr;
	}

	void WindowBase::SetApp(DemoAppBase* app)
	{
		app_ = app;
	}

	void* WindowBase::GetHandle() const
	{
		return g_hwnd;
	}

	void WindowBase::Create(uint32 width, uint32 height, const char* title, bool wndmode)
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
			CHECK_WIN_ERROR();
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
			CHECK_WIN_ERROR();
			return;
		}

		g_Window = this;

		width_ = width;
		height_ = height;
		wndmode_ = wndmode;

		ShowWindow(g_hwnd, SW_SHOW);
		UpdateWindow(g_hwnd);

		OnCreate();
	}

	void WindowBase::CreateFullScreen(const char* title)
	{
		int cx_screen = GetSystemMetrics(SM_CXSCREEN);
		int cy_screen = GetSystemMetrics(SM_CYSCREEN);

		Create(cx_screen, cy_screen, title, false);
	}

	bool WindowBase::MsgProc(void* hwnd, uint32 uMsg, uint64 wParam, uint64 lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			KeyPressEvent event((int)wParam);
			app_->OnEvent(event);
			InputState::OnKeyPress(event);
			if (event.GetKey() == KEY_ESCAPE)
				SendMessage((HWND)hwnd, WM_CLOSE, 0, 0);
		}
		break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			KeyReleaseEvent event((int)wParam);
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

	bool WindowBase::Loop()
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
				float ts = clamp_t(timer_.Tick(), 0.0f, 1.0f);
				Timestep timestep(ts);

				app_->OnUpdate(timestep);
				app_->OnRender(timestep);

				timer_.Reset();
			}
		}

		app_->Destroy();

		OnDestroy();

		return false;
	}
}